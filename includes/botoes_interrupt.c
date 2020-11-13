// Pinos
#define A_RFID  GPIO_NUM_13
#define RELE  GPIO_NUM_12
#define BT_SEG  GPIO_NUM_32
#define BT_SENS  GPIO_NUM_34
#define BT_DESTRVAR  GPIO_NUM_35
#define PIN_RFID GPIO_NUM_15

// Descritor
#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<A_RFID) | (1ULL<<RELE))
#define GPIO_INPUT_PIN_SEL  ((1ULL<<BT_DESTRVAR) | (1ULL<<BT_SENS))

/*#define ESP_INTR_FLAG_DEFAULT 0
void IRAM_ATTR destravar_porta(void* arg){
  uint32_t status = 0;
  if( gpio_get_level(BT_DESTRVAR) == 1 && status == 0 )
  {
    if( gpio_get_level(BT_DESTRVAR) == 1 && status == 0 )
    {
      //Serial.println("foi");
      status = 1;
    }
  }
}*/

// Tarefa que monita o estado da porta, fechada ou aberta
void xSensorMag(void *pvParameter){
  uint32_t status = 0;
  while(true){
    if( gpio_get_level(BT_SENS) == 1 && status == 0){
      vTaskDelay( 100 / portTICK_PERIOD_MS );
    	if( gpio_get_level(BT_SENS) == 1 && status == 0 ){
        //Serial.println("Fechou a porta");
        gpio_set_level(RELE, 0);
        audio(0, "parar");
    		status = 1;
    	}
    }
    if( gpio_get_level(BT_SENS) == 0 && status == 1 ){
      vTaskDelay( 100 / portTICK_PERIOD_MS );
    	if( gpio_get_level(BT_SENS) == 0 && status == 1 ){
        //Serial.println("A porta foi aberta");
        xTempoPortaInicio = xTimerStart(xTempoPorta, 0);
    		status = 0;
    	}
    }
    ////Serial.print("botao: "); //Serial.println(uxTaskGetStackHighWaterMark (NULL));
    vTaskDelay(100/portTICK_PERIOD_MS );
  }
}

// Tarefa que monitora o botao de destravamento da porta
void xDestravar(void *pvParameter){
  uint32_t status = 0;
  while(true){
    if( gpio_get_level(BT_DESTRVAR) == 1 && status == 0){
      vTaskDelay( 100 / portTICK_PERIOD_MS );
    	if( gpio_get_level(BT_DESTRVAR) == 1 && status == 0 ){
        //Serial.println("Porta Destravada");
        gpio_set_level(RELE, 1); // Levanta o pino do RELE [ligando ele]
    		status = 1;
    	}
    }
    if( gpio_get_level(BT_DESTRVAR) == 0 && status == 1 ){
      vTaskDelay( 100 / portTICK_PERIOD_MS );
    	if( gpio_get_level(BT_DESTRVAR) == 0 && status == 1 ){
        //Serial.println("Porta Abriu");
        gpio_set_level(RELE, 0); // Abaixa o pino do RELE [desligando ele]
    		status = 0;
    	}
    }
    ////Serial.print("rfid: "); //Serial.println(uxTaskGetStackHighWaterMark (NULL));
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

// Configura os pinos entrada e saida
void botoes_interrupt(){
  // Descritor da configuração das saídas
  gpio_config_t io_conf;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  io_conf.mode = GPIO_MODE_OUTPUT;
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  gpio_config(&io_conf);
  gpio_set_level(A_RFID, 1); // Levanta o pino do RFID [ligando ele]
  gpio_set_level(RELE, 0); // Abaixa o pino do RELE [inicia desligado]

  // Descritor da configuração dos botões como entrada
  gpio_config_t io_conf2;
  io_conf2.intr_type = GPIO_INTR_DISABLE;
  io_conf2.mode = GPIO_MODE_INPUT;
  io_conf2.pin_bit_mask = GPIO_INPUT_PIN_SEL;
  io_conf2.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf2.pull_up_en = GPIO_PULLUP_DISABLE;
  gpio_config(&io_conf2);

  // Configuraçao futura do botão de SEGURANÇA, configurado como um INTERRUPTOR
  /*gpio_set_direction(BT_SEG, GPIO_MODE_INPUT);
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  gpio_set_intr_type(BT_SEG, GPIO_INTR_NEGEDGE);
  gpio_intr_enable(BT_SEG);
  gpio_isr_handler_add(BT_SEG, destravar_porta, NULL);*/
}
