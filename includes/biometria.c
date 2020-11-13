HardwareSerial DigitalL(2); //RX2 ==> Amarelo || TX2 ==> Branco
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&DigitalL);

void iniciar_biometria() {
  OledPrincipal("boot", "biometria", "", "");
  finger.begin(57600);
  if (finger.verifyPassword()) {
    //exibir("normal", "OK", "Encontrado", "", 0);
  }else{
    //exibir("normal", "Nao", "Encontrado", "", 0);
    while (1);
  }
  vTaskDelay(1000/portTICK_PERIOD_MS );
}

// ====================================================================
// ======================= BIOMETRIA ==================================
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Imagem Capturada");
      break;
    case FINGERPRINT_NOFINGER:
      ////Serial.println("Dedo nao Localizado");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      //Serial.println("Erro ao Capturar");
      return p;
    default:
      //Serial.println("Erro desconhecido");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      //Serial.println("Imagem Convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      //Serial.println("Imagem muito confusa");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      //Serial.println("Erro ao se comunicar");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      //Serial.println("Impossivel localizar Digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      //Serial.println("Impossivel Localizar Digital");
      return p;
    default:
      //Serial.println("Erro Desconhecido");
      return p;
  }

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    //Serial.println("Digital Encontrada");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //////Serial.println("Erro ao se Comunicar");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    //Serial.println("Digital Desconhecida");
    //Serial.println("Acesso Negado.");
    audio(7, "som");
    OledPrincipal("normal", "", "Acesso", "Negado");
    vTaskDelay(1500/portTICK_PERIOD_MS);
    conteudo_oled = 0;
    return p;
  } else {
    //Serial.println("Erro Desconhecido");
    //exibir("Erro", "Desconhecido");
    return p;
  }
  //Serial.print("ID # Encontrado");
  //Serial.print(finger.fingerID);
  String pegarID = String(finger.fingerID);
  //Serial.println("Aprovado.");
  audio(0, "bemvindo"); // Mensagem em audio de boas vindas
  gpio_set_level(RELE, 1);
  xTempoReleInicio = xTimerStart(xTempoRele, 0);
  OledPrincipal("normal", "", "Acesso", "Liberado");
  String MAC_WIFI = String(mac[0], HEX)+String(mac[1], HEX)+String(mac[2], HEX)+String(mac[3], HEX)+String(mac[4], HEX)+String(mac[5], HEX);
  String dados_historico = horario+"&dados="+pegarID;
  xQueueSend(xQueue_Historico, &dados_historico, (10/portTICK_PERIOD_MS)); // Envia o histórico de entrada para fila
  //Serial.print(" com precisao de ");
  //Serial.println(finger.confidence);
  vTaskDelay(1500/portTICK_PERIOD_MS);
  conteudo_oled = 0;
  return finger.fingerID;
}
// ======================= BIOMETRIA ==================================
// ====================================================================
// ======================= CADASTRO ===================================
int cadastro_bio(String biometria, int id){
  packet2[128] = {0}; packet3[128] = {0}; packet4[128] = {0}; packet5[128] = {0}; int p;
  Serial.println(biometria);
  char char_bio[3080] = { 0 };
  biometria.toCharArray(char_bio, 3080);
  char* pack[4];
  pack[0] = strtok(char_bio, "."); pack[1] = strtok(NULL, "."); pack[2] = strtok(NULL, "."); pack[3] = strtok(NULL, ".");

  String packet_bio[4];
  packet_bio[0] = pack[0]; packet_bio[1] = pack[1]; packet_bio[2] = pack[2]; packet_bio[3] = pack[3];
  Serial.println(packet_bio[0]); Serial.println(packet_bio[1]); Serial.println(packet_bio[2]); Serial.println(packet_bio[3]);

  char buff[tamanho_bio] = { 0 }; char* buff_pack[130] = { 0 };
  for(int a=0; a<4; a++){
    packet_bio[a].toCharArray(buff, tamanho_bio);

    buff_pack[0] = strtok(buff, "_");
    if(a == 0){
          packet2[0] = (uint8_t)atof(buff_pack[0]);
    }else if(a == 1){
          packet3[0] = (uint8_t)atof(buff_pack[0]);
    }else if(a == 2){
          packet4[0] = (uint8_t)atof(buff_pack[0]);
    }else if(a == 3){
          packet5[0] = (uint8_t)atof(buff_pack[0]);
    }

    int n_bio=1;
    for(int i=1; i < 128; i++) {
      vTaskDelay(10/portTICK_PERIOD_MS);
      if(buff_pack != NULL){
        buff_pack[n_bio] = strtok(NULL, "_");
        if(a == 0){
          packet2[i] = (uint8_t)atof(buff_pack[n_bio]);
        }else if(a == 1){
          packet3[i] = (uint8_t)atof(buff_pack[n_bio]);
        }else if(a == 2){
          packet4[i] = (uint8_t)atof(buff_pack[n_bio]);
        }else if(a == 3){
          packet5[i] = (uint8_t)atof(buff_pack[n_bio]);
        }
        n_bio++;
      }
    }
    buff[tamanho_bio] = { 0 }; buff_pack[130] = { 0 };
  }

  p = finger.uploadModelPercobaan(packet2, packet3, packet4, packet5);   // Simpan di Char Buffer 01
 switch (p) {
   case FINGERPRINT_OK:
     //Serial.println(" SUKSES");
     //Serial.read();
     break;
   case FINGERPRINT_PACKETRECIEVEERR:
     //Serial.println("Communication error");
     return p;
   case FINGERPRINT_BADPACKET:
     //Serial.println("Bad packet");
     return p;
   default:
 {
 Serial.println("\n==>[SUKSES] UploadModel = ");
     //return p;
 }
 }
 Serial.print("\n==>[SUKSES] StoreModel + ID = ");
 p = finger.storeModel(id);                  // taruh di ID = 0 pada flash memory FP
 if (p == FINGERPRINT_OK) {
   //Serial.println(" Stored!");
   conteudo_oled = 0;
 } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
   //Serial.println("Communication error");
   conteudo_oled = 0;
   return p;
 } else if (p == FINGERPRINT_BADLOCATION) {
   //Serial.println("Could not store in that location");
   conteudo_oled = 0;
   return p;
 } else if (p == FINGERPRINT_FLASHERR) {
   //Serial.println("Error writing to flash");
   conteudo_oled = 0;
   return p;
 } else {
   //Serial.println("Unknown error");
   conteudo_oled = 0;
   return p;
 }
}
// ======================= CADASTRO ===================================
// ====================================================================

// ====================================================================
// ======================= APAGANDO ===================================
uint8_t deleteFingerprint(uint8_t id) {
  int p = -1;
  p = finger.deleteModel(id);
  int tempo_del = 1000;
  if (p == FINGERPRINT_OK) {
    //Serial.println("Deleted!");
    vTaskDelay(tempo_del/portTICK_PERIOD_MS);
    conteudo_oled = 0;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    //Serial.println("Communication error");
    vTaskDelay(tempo_del/portTICK_PERIOD_MS);
    conteudo_oled = 0;
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    //Serial.println("Could not delete in that location");
    vTaskDelay(tempo_del/portTICK_PERIOD_MS);
    conteudo_oled = 0;
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    //Serial.println("Error writing to flash");
    vTaskDelay(tempo_del/portTICK_PERIOD_MS);
    conteudo_oled = 0;
    return p;
  } else {
    //Serial.print("Unknown error: 0x"); //Serial.println(p, HEX);
    vTaskDelay(tempo_del/portTICK_PERIOD_MS);
    conteudo_oled = 0;
    return p;
  }
}
// ======================= APAGANDO ===================================
// ====================================================================
