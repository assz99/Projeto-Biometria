HardwareSerial PlayerMP3(0);
DFRobotDFPlayerMini myDFPlayer;

// Inicia modulo audio
void iniciar_audio(){
  OledPrincipal("boot", "audio", "", "");
  PlayerMP3.begin(9600, SERIAL_8N1, 3, 1);  // speed, type, RX, TX
  if (!myDFPlayer.begin(PlayerMP3))
  {
    //while (true);
  }else{
    //Definicoes iniciais MP3
    myDFPlayer.setTimeOut(500); //Timeout serial 500ms
    myDFPlayer.volume(30); //Volume
    myDFPlayer.EQ(0); //Equalizacao normal
    myDFPlayer.stop();
  }
}

// Função que faz chamada de audio
void audio(int n, String acao){
  xSemaphoreTake(xMutexAudio, portMAX_DELAY);
    if(acao == "som"){ // Condiçao de iniciar algum som
      myDFPlayer.playMp3Folder(n);
    }else if(acao == "parar"){  // Condiçao PARAR o audio
      myDFPlayer.stop();
    }else if(acao == "bemvindo"){ // Condiçao de bem vindo
      if ((hora >= (int)00) and (hora  <= (int)11)) {
        myDFPlayer.playMp3Folder(1);
        //Serial.println("Bom dia");
      }else if ((hora >= (int)12) and (hora <= (int)17)) {
        myDFPlayer.playMp3Folder(2);
        //Serial.println("Boa tarde");
      }else if ((hora >= (int)18) and (hora <= (int)23)) {
        myDFPlayer.playMp3Folder(3);
        //Serial.println("Boa noite");
      }
    }
  xSemaphoreGive(xMutexAudio);
}
