// Cabeçalhos
#include "includes/headers.h"

void setup(){
// -----------------------------------------------------------------------
  // Inicialização da memória não volátil para armazenamento de dados (Non-volatile storage (NVS)).
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  iniciar_wifi(); // Inicia o WIFI
// -----------------------------------------------------------------------
  // Mutex
  xMutexOled = xSemaphoreCreateMutex();
  xMutexAudio = xSemaphoreCreateMutex();
  xMutexHTTP = xSemaphoreCreateMutex();
// -----------------------------------------------------------------------
  // Criando grupos de eventos
  wifi_event_group = xEventGroupCreate();
// -----------------------------------------------------------------------
  // Timers
  xTempoRele = xTimerCreate( "xTempoRele", tempoRELE, pdFALSE, 0, pvTempoReleCallback);
  xTempoPorta = xTimerCreate( "xTempoPorta", tempoPORTA, pdFALSE, 0, pvTempoPortaCallback);
// -----------------------------------------------------------------------
  //Serial.begin(9600); // Inicia a Serial
// -----------------------------------------------------------------------
  // Criando fila
  xQueue_Historico = xQueueCreate(5, sizeof(char *));
// -----------------------------------------------------------------------
  // Inicializadores
  botoes_interrupt(); // Inicia os interruptores
  iniciar_spiffs(); // Inicia o sistema SPIFFS
  iniciar_oled(); // Inicia o módulo de OLED
// -----------------------------------------------------------------------
  // Tarefas
  xTaskCreatePinnedToCore(xOLED,"xOLED", (30*1024), NULL,  5, NULL, 1); // Tarefa 1
  xTaskCreate(xRFID,"xRFID",(30*1024),NULL,4,NULL); // Tarefa 2
  xTaskCreate(xFila,"xFila", (15*1024), NULL, 3, NULL); // Tarefa 3
  xTaskCreatePinnedToCore(xRTC,"xRTC", (15*1024), NULL, 3, NULL, 1); // Tarefa 4
  xTaskCreate(xSensorMag,"xSensorMag", (12*1024), NULL, 3, NULL); // Tarefa 5
  xTaskCreate(xDestravar,"xDestravar", (12*1024), NULL, 3, NULL); // Tarefa 6
  xTaskCreate(xVerificacao,"xVerificacao", (25*1024), NULL, 3, NULL); // Tarefa 7
}

void loop(){
  vTaskDelay(100/portTICK_PERIOD_MS);
}
