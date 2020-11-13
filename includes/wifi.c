//#define CON_WIFI_SSID      CONFIG_ESP_WIFI_SSID
//#define CON_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
static const char *TAG = "WIFI: ";
int idS = 1;
//static int tentativas = 0;

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch(event->event_id) {
		case SYSTEM_EVENT_STA_START:
      esp_wifi_connect();
		 break;
		case SYSTEM_EVENT_STA_GOT_IP:
			ESP_LOGI(TAG, "got ip:%s", ip4addr_ntoa(&event->event_info.got_ip.ip_info.ip));
      ESP_ERROR_CHECK(esp_read_mac(mac, ESP_MAC_WIFI_STA));
      //tentativas = 0;
			xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
      if(idS == 1){
        conteudo_oled = 0;
        idS = 0;
      }
			break;
		case SYSTEM_EVENT_STA_DISCONNECTED:
      //if (tentativas < 40) {
           //esp_wifi_stop();
           //esp_wifi_start();
			     esp_wifi_connect();
           xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
           //tentativas++;
           if(idS == 0){
             conteudo_oled = 0;
             idS = 1;
           }
      /*}else{
        ESP.restart();
      }*/
			break;
		default:
			break;
    }
    return ESP_OK;
}

void iniciar_wifi(){
  tcpip_adapter_init();
	tcpip_adapter_dhcpc_stop(TCPIP_ADAPTER_IF_STA);
	tcpip_adapter_ip_info_t ipInfo;
  tcpip_adapter_dns_info_t dns_info;
  IP_ADDR4(&dns_info.ip, IP_dns[0],IP_dns[1],IP_dns[2],IP_dns[3]); // ALTERAR SE FOR NECESSÁRIO
	IP4_ADDR(&ipInfo.ip, IP_wifi[0],IP_wifi[1],IP_wifi[2],IP_wifi[3]); // ALTERAR SE FOR NECESSÁRIO
	IP4_ADDR(&ipInfo.gw, IP_gw[0],IP_gw[1],IP_gw[2],IP_gw[3]); // ALTERAR SE FOR NECESSÁRIO
	IP4_ADDR(&ipInfo.netmask, IP_mask[0],IP_mask[1],IP_mask[2],IP_mask[3]); // ALTERAR SE FOR NECESSÁRIO
	tcpip_adapter_set_ip_info(TCPIP_ADAPTER_IF_STA, &ipInfo);
  tcpip_adapter_set_dns_info(TCPIP_ADAPTER_IF_STA, TCPIP_ADAPTER_DNS_MAIN, &dns_info);

  ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL) );

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  wifi_config_t wifi_config = {
      .sta = {
          {.ssid = CON_WIFI_SSID},
          {.password = CON_WIFI_PASS}
      },
  };
  //ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
  //ESP_ERROR_CHECK(esp_wifi_start());

  ESP_LOGI(TAG, "wifi_init_sta finished.");
  ESP_LOGI(TAG, "connect to ap SSID:%s password:%s",CON_WIFI_SSID, CON_WIFI_PASS);
}
