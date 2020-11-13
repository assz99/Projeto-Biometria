// Dados do Servidor
String endereco = "192.168.1.3"; // IP do Servidor [HTTP] [MQTT]
String porta_mqtt = "1883"; // Porta do MQTT

// Tópico do MQTT
#define TOPICO_BIO "sala/biometria_e117/comando" // Nome do topico usado em nossa REDE [MQTT]

// Dados de Conexao WIFI
#define CON_WIFI_SSID 	"Cleunice"
#define CON_WIFI_PASS 	"28467591"
uint8_t IP_dns[4] = {192,168,1,1}; // IP do DNS[do gateway]
uint8_t IP_wifi[4] = {192,168,1,240}; // IP do dispositivo
uint8_t IP_gw[4] = {192,168,1,1}; // IP do gateway
uint8_t IP_mask[4] = {255,255,255,0}; // IP Mascara

//======================================================
//================ NAO ALTERAR OS DADOS ================
String local = "controle";
String enviar_rfid = "http://"+endereco+"/"+local+"/acoes.php?env_rfid=";
String banco_rfid = "http://"+endereco+"/"+local+"/acoes.php?banco_rfid=";
String up_horario = "http://"+endereco+"/"+local+"/horario.php";
String historico = "http://"+endereco+"/"+local+"/acoes.php?env_historico&data=";
String end_biometria = "http://"+endereco+"/"+local;
String esp_mac = "http://"+endereco+"/"+local+"/cadastrar_esp.php?cadastro_esp&mac=";
String up_bio_cad = "http://"+endereco+"/"+local+"/acoes.php?banco_bio_cad=";
String up_bio_del = "http://"+endereco+"/"+local+"/acoes.php?banco_bio_del=";
//======================================================
#define tempoRELE	(pdMS_TO_TICKS(500UL))
#define tempoPORTA	(pdMS_TO_TICKS(10000UL))
// Tempo de Verificação das tarefas relacionadas ao banco de dados
#define tempoVerficacao  (8*60000) // 8min
//================ NAO ALTERAR OS DADOS ================
//======================================================
