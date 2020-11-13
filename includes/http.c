// Função que possibilita o ESP32 acessar o banco de dados mysql
void http_url(String url, String tipo){
  xSemaphoreTake(xMutexHTTP, portMAX_DELAY);
  int idw = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, 0);
  if(idw == 1){
    if(tipo.equals("env_mac")){
      http.begin(url);
      http.GET();
      resultado = http.getString();
      http.end();
    }else if(tipo.equals("env_historico")){
      String MAC_WIFI = String(mac[0], HEX)+String(mac[1], HEX)+String(mac[2], HEX)+String(mac[3], HEX)+String(mac[4], HEX)+String(mac[5], HEX);
      String tempH = ""; String url_dados = "";
      for(int i=0;i<(50);i++){
        if(temp_historico[i] != ""){
          tempH = temp_historico[i];
          url_dados = url+tempH+"&mac="+MAC_WIFI;
          http.begin(url_dados);
          int code = http.GET();
          String conf = String(http.getString());
          if(code == 200 and conf == "OK"){
            temp_historico[i] = {""};
          }
          http.end();
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
      }
    }else if(tipo.equals("env_info")){
      http.begin(url);
      http.GET();
      http.end();
    }else if(tipo.equals("atualizar_bio_cad")){
      http.begin(url);
      int code = http.GET();
      char msg_dados[3110];
      String dadosBIOCad = String(http.getString());
      dadosBIOCad.toCharArray(msg_dados, 3110);
      int n_upR = strlen(msg_dados);
      if((n_upR >= 2000) and (dadosBIOCad != "OK")){
        if(code == 200){
          conteudo_oled = 5;
          char * Info[2];
          Info[0] = strtok (msg_dados, "?");
          Info[1] = strtok (NULL, "?");
          cadastro_bio(String(Info[1]), atof(Info[0]));
        }
      }
      http.end();
    }else if(tipo.equals("atualizar_bio_del")){
      http.begin(url);
      int code = http.GET();
      char msg_dados[5];
      String dadosBIODel = String(http.getString());
      dadosBIODel.toCharArray(msg_dados, 5);
      int n_upR = strlen(msg_dados);
      if((n_upR >= 1) and (dadosBIODel != "OK")){
        if(code == 200){
          conteudo_oled = 5;
          vTaskDelay(1000/portTICK_PERIOD_MS);
          deleteFingerprint((uint8_t)(dadosBIODel.toInt()));
        }
      }
      http.end();
    }else if(tipo.equals("atualizar_rfid")){
      http.begin(url);
      int code = http.GET();
      char msg_dados[2048];
      String dadosRF = String(http.getString());
      dadosRF.toCharArray(msg_dados, 2048);
      int n_upR = strlen(msg_dados);
      if((n_upR >= 10) and (dadosRF != "OK")){
        if(code == 200){
          FILE* f = fopen("/spiffs/rfid.txt", "w+");
          if (f == NULL) {
            //ESP_LOGE(TAG_spiffs, "Failed to open file for writing");
            return;
          }
          fprintf(f, msg_dados);
          fclose(f);
          get_rfid();
        }
      }
      http.end();
    }else if(tipo.equals("atualizar_horario")){
      http.begin(url);
      int code = http.GET();
      if(code == 200){
        //Serial.println("Ajustando o Horario");
        String horarioAT = String(http.getString());
        char HORAchar[25];
        horarioAT.toCharArray(HORAchar, 25);
        int n_upH = strlen(HORAchar);
        if(n_upH >= 18){
          char * InfoHORA[6];
          InfoHORA[0] = strtok (HORAchar, "?");
          InfoHORA[1] = strtok (NULL, "?");
          InfoHORA[2] = strtok (NULL, "?");
          InfoHORA[3] = strtok (NULL, "?");
          InfoHORA[4] = strtok (NULL, "?");
          InfoHORA[5] = strtok (NULL, "?");
          int a = atof(InfoHORA[0]); int b = atof(InfoHORA[1]); int c = atof(InfoHORA[2]); int d = atof(InfoHORA[3]); int e = atof(InfoHORA[4]); int f = atof(InfoHORA[5]);
          if (rtc_ON == 1) {
            rtc.adjust(DateTime(a, b, c, d, e, f));
            rtc_OK = 1;
            //Serial.println("Pronto, horario ajustado.");
          }
        }
      }
      http.end();
    }
  }
  xSemaphoreGive(xMutexHTTP);
}
