
void time_control(){
  
  if(time_act){
    if(millis() - start_time >= set_hours * 3600000.00){
      time_act = false;
      OnOff = false;
    }else{
      left_hours = set_hours- (millis() - start_time)/3600000.00;
    }
  }
}

