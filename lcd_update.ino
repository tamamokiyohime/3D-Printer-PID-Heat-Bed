void lcd_update(){
  
    //ROW 1
    lcd.setCursor(0,0);   lcd.print(F("Temp="));   lcd.print(temperature,2);  lcd.print(F("/"));   lcd.print(setPoint,2);
    if(time_act){
        lcd.setCursor(15,1); lcd.write((byte)6); lcd.print(left_hours,2);
    }
    if(!time_act){
      lcd.setCursor(15,1); lcd.print(F("     "));
    }
    if(myPID.isStopped()){
      lcd.setCursor(0,2); lcd.print(F("----PID  Stopped----"));
      lcd.setCursor(0,3); lcd.print(F("Press Encoder to ReS"));
    }
  
    //PWM State
    if(!myPID.isStopped()){
      lcd.setCursor(0,2); 
      switch(running_status)
      {
        case 1:
        lcd.print(F("--- PID  Running ---"));
        running_status = 2;
        break;
        case 2:
        lcd.print(F("--< PID  Running >--"));
        running_status = 3;
        break;
        case 3:
        lcd.print(F("-<- PID  Running ->-"));
        running_status = 4;
        break;
        case 4:
        lcd.print(F("<-- PID  Running -->"));
        running_status = 1;
        break;
      }
      
      int PWMS = outputVal/(OUTPUT_MAX/20);
      lcd.setCursor(0,3);  //lcd.print();
      for(int k = 0 ; k<20 ; k++){
        if(k < PWMS)   lcd.write((byte)0);
        else           lcd.print(F(" "));
        }
    }
}

