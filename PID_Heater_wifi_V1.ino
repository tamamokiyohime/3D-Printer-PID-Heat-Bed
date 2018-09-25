#include <AutoPID.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <EEPROM.h>
#include "Button2.h";
#include "ESPRotary.h";

//System Setting====================================
#define temp_sample_times 	3					//Temperature Sample Time
#define heater_top_limit 	90					//Heating Top Limit
#define heater_low_limit 	30					//Heating Low Limit
#define TEMP_READ_DELAY 	500					//Set PID Operate Time (ms)
#define LCD_interval 		500					//Set LCD Update Time (ms)
#define DB_interval 		10000				//Set DB Update Time (ms)
double  setPoint	  = 	70.00;  			//default Set Point Temperature
char    tempCoe[]     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};  //Average Temperature Product

//Time Action=====================
unsigned long lastTempUpdate = 0;
unsigned long lastDBupdate   = 0;
unsigned long lastLCDUpdate  = 0;



//Firebase Setting==================================================
#define FIREBASE_HOST "pid-heater.firebaseio.com"
#define FIREBASE_AUTH "Xi7CEpIxfd8PTt8Jml94VkEmAgUaqgpS6Nz2Qqpu"
#define WIFI_SSID "HOME"
#define WIFI_PASSWORD "0970278815"

float	DB_last_temp = 0;
boolean first_DB_connect_temp = false;




//Rotary Enc Setting==================================
#define ROTARY_PIN1  D5
#define ROTARY_PIN2  D6
#define BUTTON_PIN   D4
ESPRotary r = ESPRotary(ROTARY_PIN1, ROTARY_PIN2);
Button2	  b = Button2(BUTTON_PIN);
//unsigned long button_press = 0;
//unsigned long button_release = 0;
//boolean clicked = false;
//boolean pressed = false;
boolean temp_choice = false;
boolean temp_choice_2 = false;
int oldPosition = 0;
int newPosition = 0;
int menu_pointer = 0;
int menu[3] = {0,0,0};




//System Enviroment
float set_hours = 0;
float left_hours;
boolean OnOff = true;
boolean time_act = false;
unsigned long start_time = 0;

//LiquidCrystal
LiquidCrystal_I2C lcd(0x27,20,4);
int running_status = 1;
  byte full[8] = { 
  	0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
   };
  byte wifi_yes[8] = {
    0b00000,
    0b00000,
    0b00000,
    0b11100,
    0b00010,
    0b11001,
    0b00101,
    0b10101
  }; 
  byte wifi_no[8] = {
    0b00101,
    0b00010,
    0b00101,
    0b11100,
    0b00010,
    0b11001,
    0b00101,
    0b10101
  }; 
  byte DB_U[8] = {
    0b11000,
    0b10100,
    0b10100,
    0b11000,
    0b00111,
    0b00011,
    0b00101,
    0b01000
  }; 
  byte DB_D[8] = {
    0b11000,
    0b10100,
    0b10100,
    0b11000,
    0b00001,
    0b01010,
    0b01100,
    0b01110
  };
  byte DB_X[8] = {
    0b11000,
    0b10100,
    0b10100,
    0b11000,
    0b00000,
    0b00101,
    0b00010,
    0b00101
  };
  byte time_stop[8] = {
    0b00000,
    0b00100,
    0b00100,
    0b10101,
    0b10101,
    0b10001,
    0b01110,
    0b00000
  };

//PID Enviroment Setting============================================
#define OUTPUT_PIN D0
#define OUTPUT_MIN 0
#define OUTPUT_MAX 1020
#define KP 3500
#define KI 2.0
#define KD 0
double  temperature, outputVal;
AutoPID myPID(&temperature, &setPoint, &outputVal, OUTPUT_MIN, OUTPUT_MAX, KP, KI, KD);



//temperature=======================================================
SoftwareSerial MCU(D7,D8);
unsigned char  response[]    = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  static byte  readtemp[]	 = {0xA5, 0x15, 0xBA}; //requesting data
  static byte  pooled_Mode[] = {0xA5, 0x52, 0xF7};
		 float obj_temp 	 = 0;
		 float sur_temp 	 = 0;
unsigned int   check_sum;
		 float Ave_10;
		 char  last10Temp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


//PC Command Decode=====================================================
String  inputstring = "";                              //a string to hold incoming data from the PC
boolean input_string_complete = false;                //have we received all the data from the PC
char 	*command;
char	*option;


//EEPROM===============================================================
int 	EEP_init = 0;
String  wifi_ssid = "kiyohime";
String  wifi_pw = "wakaba0331";


void setup() {
  Serial.begin(9600);
  inputstring.reserve(10);
  MCU.begin(9600);
  EEPROM.begin(512);
  
  
{/*LCD init Function-------------------------------*/
  lcd.init();
  lcd.backlight();
  lcd.createChar(0,full);
  lcd.createChar(1,wifi_yes);
  lcd.createChar(2,wifi_no);
  lcd.createChar(3,DB_U);
  lcd.createChar(4,DB_D);
  lcd.createChar(5,DB_X);
  lcd.createChar(6,time_stop);
  lcd.clear();
}//-------------------------------------------------
{/*Rotary Enc Function------------------------------*/
  r.setChangedHandler(rotate);
  b.setClickHandler(showPosition);
  b.setLongClickHandler(resetPosition);
}/*-------------------------------------------------*/
  wifi_connect();
{/*PID init==================================================*/
  pinMode(OUTPUT_PIN, OUTPUT); 		//pin for PID PWM output
  analogWrite(OUTPUT_PIN, 0);		//Set Defualt to 0
  while (!updateTemperature()) {} 	//wait until temp sensor updated
  myPID.setBangBang(3);   			//if temperature is more than 4 degrees below or above setpoint, OUTPUT will be set to min or max respectively
  myPID.setTimeStep(1000);  		//set PID update interval
}/*==================================================*/
  lastDBupdate = millis();
}


void loop() {
{/*Update Immediatly vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
  r.loop();
  b.loop();
  menu_event();
  time_control();
  if(OnOff)  {myPID.run();                      }//call every loop, updates automatically at certain time interval
  if(!OnOff) {myPID.stop();     outputVal = 0;  }
  analogWrite(OUTPUT_PIN, outputVal);
  if(Serial.available()>0){
    decode_command();
  }
}/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

{/*Update Temperature vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
  if(millis() - lastTempUpdate >= TEMP_READ_DELAY){
    updateTemperature();    
    printout();
    
    lastTempUpdate = millis();
  }
}/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/  

{/*Update LCD vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
  if(millis() - lastLCDUpdate >= LCD_interval){
    lcd_update();
    
    lastLCDUpdate = millis();
  }
}/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/*Update Database vvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/{
  if(millis()-lastDBupdate >= DB_interval){
    DB_update();
    DB_temperature();
    
    lastDBupdate = millis();
    lastLCDUpdate = millis();
  }
  
}
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/  
}

void rotate(ESPRotary& r) {
  newPosition = r.getPosition();
  menu[menu_pointer] += (newPosition - oldPosition);
  oldPosition = newPosition;
}

void showPosition(Button2& btn) {
  menu_pointer++;
  r.resetPosition();
  oldPosition = 0;
}

void resetPosition(Button2& btn) {
  r.resetPosition();
  oldPosition = 0;
}


