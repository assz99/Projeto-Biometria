static const int RX_BUF_SIZE = 1024;

#define TXD_PIN (UART_PIN_NO_CHANGE)
#define RXD_PIN (GPIO_NUM_15)

void iniciar_rfid() {
  OledPrincipal("boot", "wifi", "", "");
  OledPrincipal("boot", "rfid", "", "");
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
}

// Task do RFID
void xRFID(void *pvParameter){
  get_rfid(); // Atualizando o vetor com numeros rfid
  static const char *RX_TASK_TAG = "RX_TASK";
  esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
  uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);
  while (true) {
    const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
    if (rxBytes > 0) {
      data[rxBytes] = 0;
      String dados_RFID;
      char c;
      int num_c = 0;
      for(int i=0;i<rxBytes;i++){
        if (isxdigit(c=(char)(*(data+i)))){
          num_c++;
          dados_RFID += c;;
        }
      }

      // Filtrando numero de caracteres
      char rfid_tmp[13];
      dados_RFID.toCharArray(rfid_tmp, 13);
      String dados_RFID12 = String(rfid_tmp);

      // Condição de aprovação
      int aprovar = 0;
      if(pass_rfid.indexOf(dados_RFID12) != -1 and num_c > 5 and num_c < 15){
        aprovar = 1;
      }

      if(aprovar == 1 and num_c > 5 and num_c < 15){
        //Serial.println("Aprovado.");
        audio(0, "bemvindo"); // Mensagem em audio de boas vindas
        gpio_set_level(RELE, 1);
        xTempoReleInicio = xTimerStart(xTempoRele, 0);
        conteudo_oled = 4; rfid_visor = 1;
        String MAC_WIFI = String(mac[0], HEX)+String(mac[1], HEX)+String(mac[2], HEX)+String(mac[3], HEX)+String(mac[4], HEX)+String(mac[5], HEX);
        String dados_historico = horario+"&dados="+dados_RFID12;
        xQueueSend(xQueue_Historico, &dados_historico, (10/portTICK_PERIOD_MS)); // Envia o histórico de entrada para fila
      }else{
        //Serial.println("Acesso Negado.");
        audio(7, "som");
        conteudo_oled = 4; rfid_visor = 2;
      }
      dados_RFID = ""; // Limpando buffer
      vTaskDelay(1500/portTICK_PERIOD_MS);
      conteudo_oled = 0;
      reniciar_RFID(); // Reinicia o RFID
    }
    ////Serial.print("rfid: "); //Serial.println(uxTaskGetStackHighWaterMark (NULL));
    vTaskDelay(50/portTICK_PERIOD_MS);
  }
  free(data); // Limpando buffer
}

// -----------------------------------------------------------------------
void reniciar_RFID(){
  gpio_set_level(A_RFID, 0);
  vTaskDelay(600/portTICK_PERIOD_MS);
  gpio_set_level(A_RFID, 1);
}
