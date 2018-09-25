float getValue(byte packet[]){
  MCU.read();
  check_sum = 0;
  while(!MCU.available()){
    MCU.write(readtemp,3);
    delay(10);
       }
  int timeout = 0;
    while (MCU.available()<9){
    timeout++;
    if(timeout > 10) {
      MCU.read();
      MCU.write(readtemp,3);
      timeout = 0;
      }
    delay(10);
   }
  for (int i = 0; i < 9; i++) {response[i] = MCU.read();}
  float high_obj = packet[4];
  float  low_obj = packet[5];
  return (high_obj * 256 + low_obj)/100;
}

