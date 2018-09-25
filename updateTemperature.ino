bool updateTemperature() 
{
    float sum = 0.00;
    int coe_total = 0;
    float temp_space = 0.00;
    
    for(int times = 0; times < temp_sample_times; times++){
      temp_space += getValue(response);
    }
    temperature = temp_space/temp_sample_times;
    
    for (int i = 0; i<9;i++){    //shift temperature array forward
          last10Temp[i] = last10Temp[i+1];
    }
    
    last10Temp[9] = temperature;
 
    for (int j = 0; j<=9;j++){
      sum += last10Temp[j]*tempCoe[j];
      coe_total += tempCoe[j];
    }
    
    Ave_10 =  sum/coe_total;
    
    return true;
}

