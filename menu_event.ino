void menu_event(){
   
   int menu_select = abs(menu[0])%16/4;
   lcd.setCursor(0,1);
   
   
   switch(menu_select){
     {case 0:
      lcd.print("Set Temp");
      if(menu_pointer == 1){
        lcd.print(">");
        lcd.print(setPoint+menu[1],2);
        if((setPoint+menu[1])>heater_top_limit) menu[1] = 90-setPoint;
        if((setPoint+menu[1])<heater_low_limit) menu[1] = 30-setPoint;
      }
      if(menu_pointer == 2){
        lcd.print("            ");
        setPoint += menu[1];
        menu_pointer =0;
        menu[1] = 0;
        menu[2] = 0;
      }
    break;}
    {case 1:
      lcd.print("Set Time");
      if(menu_pointer == 1){
        lcd.print(">");
        lcd.print(menu[1]/10.00,2);
        if((menu[1])<0) menu[1] = 0;
      }
      if(menu_pointer == 2){
        lcd.print("            ");
        set_hours = menu[1]/10.00;
        if(set_hours > 0)  {time_act = true; start_time = millis();}
        menu_pointer =0;
        menu[1] = 0;
        menu[2] = 0;
      }
    break;}
    {case 2:
    
      lcd.print("Res Time");
      if(menu_pointer == 1){
        lcd.print(">");
        if(abs(menu[1])%7<=3) {lcd.print("YES"); temp_choice = true; }
        if(abs(menu[1])%7>=4) {lcd.print("NO "); temp_choice = false;}
      }
      if(menu_pointer == 2){
        lcd.print("            ");
        if(temp_choice) {time_act = false; set_hours = 0; start_time = millis();}
        menu_pointer =0;
        menu[1] = 0;
        menu[2] = 0;
      }
    break;}
    {case 3:
      lcd.print("Sys Idle");
      if(menu_pointer == 1){
        lcd.print(">");
        if(abs(menu[1])%7<=3) {lcd.print("Idle"); temp_choice_2 = true; }
        if(abs(menu[1])%7>=4) {lcd.print("Run "); temp_choice_2 = false;}
      }
      if(menu_pointer == 2){
        lcd.print("            ");
        if(temp_choice_2)  OnOff = false;
        if(!temp_choice_2) OnOff = true;
        menu_pointer =0;
        menu[1] = 0;
        menu[2] = 0;
      }
    break;}
   }
}
