//////////////////////////////////////////////
// Verificaçao do banco de dados, se há informações novas ou não
//////////////////////////////////////////////
void xVerificacao(void *pvParameter){
  TickType_t tempo, inicio, fim; int trava_mac = 0;
  inicio = (xTaskGetTickCount() * portTICK_PERIOD_MS)+tempoVerficacao;
  fim = (xTaskGetTickCount() * portTICK_PERIOD_MS)+inicio+(2/tempoVerficacao);
  String MAC_WIFI = "wait";
  while(true){
    tempo = xTaskGetTickCount() * portTICK_PERIOD_MS;
    int idw = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, false, true, 0);
    if(idw == 1){
      MAC_WIFI = String(mac[0], HEX)+String(mac[1], HEX)+String(mac[2], HEX)+String(mac[3], HEX)+String(mac[4], HEX)+String(mac[5], HEX);
      if(trava_mac == 0 and MAC_WIFI != "wait"){
        // Inserino MAC no banco
        resultado = "";
        http_url(esp_mac+MAC_WIFI, "env_mac");
        if(resultado == "OK"){
          trava_mac = 1;
          resultado = "";
        }
      }
    }
    if((tempo >= inicio) and (tempo <= fim)){
      int intervalo = 5000;
      if(idw == 1 and MAC_WIFI != "wait"){
        // Rotina de verificação banco de dados, se há nova BIOMETRIA cadastrada
        http_url(up_bio_cad+MAC_WIFI, "atualizar_bio_cad");
        vTaskDelay(intervalo/portTICK_PERIOD_MS);

        // Rotina de verificação banco de dados, se há BIOMETRIA apagada
        http_url(up_bio_del+MAC_WIFI, "atualizar_bio_del");
        vTaskDelay(intervalo/portTICK_PERIOD_MS);

        // Rotina de verificação banco de dados RFID, se há alguma novidade
        http_url(banco_rfid+MAC_WIFI, "atualizar_rfid");
        vTaskDelay(intervalo/portTICK_PERIOD_MS);

        // Rotina de verificação histórico de acesso
        http_url(historico, "env_historico");
        vTaskDelay(intervalo/portTICK_PERIOD_MS);
      }
    }else if((tempo > fim)){
      inicio = tempo+tempoVerficacao;
      fim = tempo+inicio+(2/tempoVerficacao);
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}
