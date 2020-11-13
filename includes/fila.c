// Tarefa que armazena o histórico de acesso
void xFila(void *pvParameter){
  char * str = NULL;
  while(true){
    xQueueReceive(xQueue_Historico, &str, portMAX_DELAY);
    String msg = String(str);
    //Serial.print("Fila: "); Serial.println(msg);
    int n = strlen(str);
    if(temp_n_H >= 50){
      temp_n_H = 0;
    }else{
      if(n >= 10 and temp_n_H < 50){
        temp_historico[temp_n_H] = {msg};
        temp_n_H++;
      }
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
    xQueueReset (xQueue_Historico);
  }
}
