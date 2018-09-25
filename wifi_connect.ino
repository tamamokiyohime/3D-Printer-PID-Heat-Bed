void wifi_connect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  int WiFi_count = 0;
  boolean WiFi_success = true;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    WiFi_count ++;
    delay(500);
    if(WiFi_count > 10){
      WiFi_success = false;
      break;
    }
  }
  Serial.println();
  lcd.setCursor(19,0);
  if(WiFi_success){
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  lcd.write((byte)1);
  }
  if(!WiFi_success){
  Serial.print("connect Fail ");
  Serial.println("Check WiFi Config");
  lcd.write((byte)2);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  delay(10);
}

