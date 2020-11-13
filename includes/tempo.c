// Temporiza o acionando do relé, evitando atrasos de outras funções
static void pvTempoReleCallback(TimerHandle_t xTempoRele){
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
  gpio_set_level(RELE, 0);
	ESP_LOGI( TAG, "Executando time do RELE. xTimeNow=%d", xTimeNow );
}

// Temporiza o acionando do audio em relação a abertura da porta
static void pvTempoPortaCallback(TimerHandle_t xTempoPorta){
	static TickType_t xTimeNow;
	xTimeNow = xTaskGetTickCount();
  audio(6, "som");
	ESP_LOGI( TAG, "Executando time da Porta. xTimeNow=%d", xTimeNow );
}
