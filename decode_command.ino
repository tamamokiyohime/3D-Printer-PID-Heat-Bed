void decode_command() {
  inputstring = Serial.readStringUntil(13);
  int len = inputstring.length();
  if (len > 0) {
    input_string_complete = true;
  }
  if (input_string_complete) {                        //if a string from the PC has been received in its entirety
    char inputstring_array[50];
    inputstring.toCharArray(inputstring_array, 50);
    command = strtok(inputstring_array, ",");
    option = strtok(NULL, ",");
    Serial.print("command = ");
    Serial.println(command);
    Serial.print("option = ");
    Serial.println(option);
    command_switch(command_convert_to_int(command));

    input_string_complete = false;
    inputstring = "";
  }
}
void command_switch(int i) {
  switch (i)
  {
    case 0:
      Serial.print("Set WiFi SSID to : ");
      wifi_ssid = String(option);
      Serial.println(wifi_ssid);
      Serial.println("New SSID hasn't been apply, use \"wifi_save\" and then \"wifi_restart\" to apply change");
      break;

    case 1:
      Serial.print("Set WiFi Password to : ");
      wifi_pw = String(option);
      Serial.println(wifi_pw);
      Serial.println("New SSID hasn't been apply, use \"wifi_save\" and then \"wifi_restart\" to apply change");
      break;

    case 2:
      Serial.print("Set Target Temperature to : ");
      if (isdigit(option[0]) == true) {
        float temp = atof(option);
        if (temp < heater_low_limit || temp > heater_top_limit) {
          Serial.println("Setting Out of Range");
        } else {
          setPoint = temp;
          Serial.println(setPoint);
        }
      } else {
        Serial.println("Wrong Option");
        Serial.println("Type \"help\" to Show All Available Command");
      }
      break;

    case 3:
      Serial.print("Now Setting SSID = ");
      Serial.println(wifi_ssid);
      Serial.print("Now Setting Password = ");
      Serial.println(wifi_pw);
      break;

    case 4:
      Serial.println("Save WiFi Config to EEPROM");
      writeEEPROM();
      break;

    case 5:
      Serial.println("Connecting to WiFi with new setting");
      wifi_connect();
      break;

    case 6:
      if (isdigit(option[0]) == true) {
        int select = atoi(option);
        if (select == 0) {
          OnOff = false;
          Serial.println("System Idle");
        } else if (select == 1) {
          OnOff = true;
          Serial.println("System Run");
        } else {
          Serial.println("Option not accept");
          Serial.println("Type \"help\" to Show All Available Command");
        }
      }else{
        Serial.println("Option not accept");
        Serial.println("Type \"help\" to Show All Available Command");
      }

      break;

    case 98:
      Serial.println("Showing Avalible Option");
      Serial.println("==WiFi==================================");
      Serial.println("Set WiFi SSID: wifi_SSID,\"STRING\"");
      Serial.println("Set WiFi PW: wifi_PW,\"STRING\"");
      Serial.println("Show WiFi Config: wifi?");
      Serial.println("Save WiFi Config to EEPROM: wifi_save");
      Serial.println("Restart WiFi With New Config: wifi_restart");
      Serial.println("==System Control========================");
      Serial.println("Set Temperature: temp,\"40.00<float<90.00\"");
      Serial.println("Set System to Idle: sys,0");
      Serial.println("Set System to Run: sys,1");
      break;

    case 99:
      Serial.println("Unknow command");
      Serial.println("Type \"help\" to Show All Available Command");
      break;
  }
}

int command_convert_to_int(String S) {
  if (S == "wifi_SSID") {
    return 0;
  }
  else if (S == "wifi_PW") {
    return 1;
  }
  else if (S == "temp") {
    return 2;
  }
  else if (S == "wifi?") {
    return 3;
  }
  else if (S == "wifi_save") {
    return 4;
  }
  else if (S == "wifi_restart") {
    return 5;
  }
  else if (S == "sys") {
    return 6;
  }
  else if (S == "help") {
    return 98;
  }
  else {
    return 99;
  }
}

