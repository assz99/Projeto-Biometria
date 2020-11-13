static const char *TAG_spiffs = "SPIFFS";

static void iniciar_spiffs() {
  ESP_LOGI(TAG_spiffs, "Inicializando o SPIFFS");
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
  };

  // Use settings defined above to initialize and mount SPIFFS filesystem.
  // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
  esp_err_t ret = esp_vfs_spiffs_register(&conf);

  if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
          ESP_LOGE(TAG_spiffs, "Falha ao montar ou formatar o sistema de arquivos");
      } else if (ret == ESP_ERR_NOT_FOUND) {
          ESP_LOGE(TAG_spiffs, "Falha ao encontrar a partição SPIFFS");
      } else {
          ESP_LOGE(TAG_spiffs, "Falha ao inicializar o SPIFFS (%s)", esp_err_to_name(ret));
      }
      return;
  }
}

// Função que atualiza o vetor com os dados de RFID
void get_rfid(){
  FILE* fRFIDr = fopen("/spiffs/rfid.txt", "r");
  if (fRFIDr == NULL) {
    ESP_LOGE(TAG_spiffs, "Failed to open file for reading");
    return;
  }
  char GET_dados[2048];
  fgets(GET_dados, sizeof(GET_dados), fRFIDr);
  fclose(fRFIDr);
  pass_rfid = String(GET_dados);
  Serial.print("rfid: "); Serial.println(pass_rfid);
}
