// Biliotecas em C
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// FreeRTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "freertos/timers.h"

// Driver
#include "driver/gpio.h"
#include <sstream>
#include "esp_log.h"

// Arduino
#include "Arduino.h"

// OLED
#include "Wire.h"
#include <U8g2lib.h>

// HTTP
#include "esp_http_client.h"
#include <HTTPClient.h> // Arduino

// Biometria
#include <Adafruit_Fingerprint.h>

// RFID
#include "esp_log.h"
#include "driver/uart.h"

// RTC
#include "RTClib.h"

// SPIFFS
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_spiffs.h"

// WIFI
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

// DFPlayer
#include "DFRobotDFPlayerMini.h"
//===================================================
//================ VARIAVEIS GLOBAIS ================
// inserção banco de dados Biometria
const int tamanho_bio = 770;
uint8_t packet2[128] = {'\0'};
uint8_t packet3[128] = {'\0'};
uint8_t packet4[128] = {'\0'};
uint8_t packet5[128] = {'\0'};

// Normal
String pass_rfid;
String temp_historico[50] = {""};
int hora, bio = 0, bio_id,  rtc_OK = 0, rtc_ON = 0, inicio_OK = 0, conteudo_oled = 0, rfid_visor = 0, temp_n_H = 0;
String horario, resultado = "";
uint8_t mac[6] = {'\0'};
const int WIFI_CONNECTED_BIT = BIT0;
HTTPClient http;
RTC_DS3231 rtc;

// MUTEX
SemaphoreHandle_t xMutexOled;
SemaphoreHandle_t xMutexAudio;
SemaphoreHandle_t xMutexHTTP;

// Grupo de Eventos
static EventGroupHandle_t wifi_event_group; // WIFI

// Handles
TimerHandle_t xTempoRele, xTempoPorta;
BaseType_t xTempoReleInicio, xTempoPortaInicio;
QueueHandle_t xQueue_Historico;

// Funções
int cadastro_bio(String biometria, int id_bio);
uint8_t getFingerprintID();
uint8_t deleteFingerprint(uint8_t id);
void OledPrincipal(String tipo, String estagio, char * texto1, char * texto2);
void xBoot(String estagio);
void audio(int n, String acao);
void rfid_leitura(String situacao);
void reniciar_RFID();
void iniciar_rfid();
void iniciar_rtc();
void iniciar_audio();
void iniciar_biometria();
void http_url(String url, String tipo);
static void pvTempoReleCallback(TimerHandle_t xTempoRele);
static void pvTempoPortaCallback(TimerHandle_t xTempoPorta);

// Estrutura Fila
/*struct SETADOR {
  char * o_texto1;
  char * o_texto2;
};
struct SETADOR dados_setador;*/
//================ VARIAVEIS GLOBAIS ================
//===================================================

// Config dos recursos
#include "config.c"
#include "botoes_interrupt.c"
#include "wifi.c"
#include "spiffs.c"
#include "tempo.c"
// #include "includes/ota.c" // Por enquanto não usaremos, estudar mais a fundo.
#include "rtc.c"
#include "oled.c"
#include "http.c"
#include "fila.c"
#include "audio.c"
#include "rfid.c"
#include "biometria.c"

#include "verificacao.c"
