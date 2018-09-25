void DB_update(){
  lcd.setCursor(17,0); lcd.write((byte)3);
  String output;
  output += "[\"";
  output += temperature;
  output += "\",\"";
  output += setPoint;
  output += "\",\"";
  output += outputVal;
  output += "\"]";

  Firebase.pushString("status", output);
  delay(100);

  lcd.setCursor(17,0);
  if (Firebase.failed()) {
      Serial.println("pushString failed");
      lcd.write((byte)5);
  }else{
    lcd.print(" ");
  }
  Serial.println("DB Uploaded");
}


void DB_temperature(){
  lcd.setCursor(18,0); lcd.write((byte)4);
  String raw_data = Firebase.getString("set_temp");
  delay(100);
  float incomming = raw_data.toFloat();

  if(incomming != 0 && incomming != DB_last_temp && first_DB_connect_temp){
  
    if(incomming == 1.00){
      OnOff = false;
      Serial.println("Database Incomming \"System Idle\"");
      
    }else if(incomming >= heater_low_limit && incomming <= heater_top_limit){
      OnOff = true;
      setPoint = incomming;
      Serial.print("Database Incomming Temperature Setting to:");
      Serial.println(incomming);
    }
    DB_last_temp = incomming;
    Serial.println(raw_data);
  }
  
  lcd.setCursor(18,0);
  if (Firebase.failed()){
    Serial.println("getString failed");
    lcd.write((byte)5);
  }else{
    lcd.print(" ");
  }


  if(!first_DB_connect_temp){
    first_DB_connect_temp = true;
    DB_last_temp = incomming;
  }
  Serial.println("DB Dowwnloaded");
//  Serial.println(DB_last_temp);
  
}

