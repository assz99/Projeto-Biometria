void iniciar_rtc() {
  OledPrincipal("boot", "rtc", "", "");
  if (! rtc.begin()) {
    //Serial.println("RTC não foi encontrado.");
    while (1);
    rtc_ON = 0;
  }else{
    rtc_ON = 1;
  }
}

void xRTC(void *pvParameter){
  int contar = 1;
  while(true){
    if(rtc_ON == 1){
      DateTime now = rtc.now();
      String horas = String(now.hour(), DEC); hora = horas.toInt();
      String dia = String(now.day(), DEC);
      if((rtc_OK == 0)){
        http_url(up_horario, "atualizar_horario"); // Ajusta o horário
      }
      if((hora > 23) or ((dia.toInt()) > 31)){ // Se perder a linha de horário, ajuste-o
        http_url(up_horario, "atualizar_horario"); // Ajusta o horário
      }
      horario = String(now.year(), DEC)+"-"+String(now.month(), DEC)+"-"+String(now.day(), DEC)+"_"+String(now.hour(), DEC)+":"+String(now.minute(), DEC)+":"+String(now.second(), DEC);
      //Serial.println(horario);
      ////Serial.print("rtc: "); //Serial.println(uxTaskGetStackHighWaterMark (NULL));
      ////Serial.print(now.day(), DEC);//Serial.print(" ");//Serial.print(now.month(), DEC);//Serial.print(" ");//Serial.print(now.year(), DEC);//Serial.print(" ");//Serial.print(now.hour(), DEC);//Serial.print(" ");//Serial.print(now.minute(), DEC);//Serial.print(" ");//Serial.println(now.second(), DEC);
    }
    // Atraso o inicio da conexão com o WIFI - diminuindo o bug no OLED
    if((inicio_OK == 1) and (contar < 6)){
      if(contar == 4){
        esp_wifi_start();
        esp_wifi_connect();
      }
      contar++;
    }
    vTaskDelay(10000/portTICK_PERIOD_MS);
  }
}
