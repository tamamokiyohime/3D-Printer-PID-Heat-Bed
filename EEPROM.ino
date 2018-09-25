void writeEEPROM() {

    int EEPAddress = EEP_init;
    EEPROM.write(EEPAddress, wifi_ssid.length());
    EEPAddress += 1;
    EEPROM.write(EEPAddress, wifi_pw.length());
    EEPAddress += 1;
    
    for(int i = 0; i<wifi_ssid.length();i++){
      EEPROM.write(EEPAddress,(int)wifi_ssid[i]);
      EEPAddress += 1;
    }
    EEPAddress += 1;
    for(int i = 0; i<wifi_pw.length();i++){
      EEPROM.write(EEPAddress,(int)wifi_pw[i]);
      EEPAddress += 1;
    }
    EEPROM.commit();
}


void readEEPROM() {
    int EEPAddress = EEP_init;
    int len_SSID = 0;
    int len_PW = 0;
    
    len_SSID = EEPROM.read(EEPAddress);
    EEPAddress += 1;
    len_PW = EEPROM.read(EEPAddress);
    EEPAddress += 1;
    
    wifi_ssid ="";
    wifi_pw = "";

    for(int i = 0; i < len_SSID; i++){
      wifi_ssid += (char)EEPROM.read(EEPAddress);
      EEPAddress += 1;
    }
    EEPAddress += 1;
    for(int i = 0; i < len_PW; i++){
      wifi_pw += (char)EEPROM.read(EEPAddress);
      EEPAddress += 1;
    }

    
  
}
