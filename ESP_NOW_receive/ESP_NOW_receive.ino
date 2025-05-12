#include <ESP8266WiFi.h>
#include <espnow.h>
#include <string.h>

//#define ESP_NOW_TRANSMITTER     1
//#define ESP_NOW_RECEIVER        1
#define ESP_NOW_BOTH            1

const uint8_t RECEIVER_MAC_ADDRESS[] = {0};
const uint8_t TRANSMITTER_MAC_ADDRESS[] = {0};

char msg_content[] = "Hello from Receiver!";
uint8_t hand_shaken_cnt = 0;
char send_msg[30] = "";
char recv_msg[30] = "";
uint32_t recv_msg_len = 0;
bool esp_now_rx_flag = false;
bool esp_now_tx_flag = false;

void esp_success_display(String func);
void esp_error_display(void);
void wifi_station_init(void);
void esp_now_initialization(void);
/*
  Return Code  Symbolic Name Meaning
   0 ESP_OK  Operation successful
  -1  ESP_FAIL  Generic failure (e.g., peer not found, send queue full)
  -2  ESP_ERR_NO_MEM  Out of memory (heap allocation failed)
  -3  ESP_ERR_INVALID_ARG Invalid argument passed (e.g., null pointers, wrong data length)
  -4  ESP_ERR_INVALID_STATE ESP-NOW not initialized or Wi-Fi not in STA/AP mode
  -5  ESP_ERR_NOT_FOUND Peer not found (when calling esp_now_del_peer or esp_now_send)
  -6  ESP_ERR_NO_PEER No peer has been added yet (send will fail)
  -7  ESP_ERR_INTERNAL  Internal error (unexpected behavior or corrupted control block)
  -8  ESP_ERR_UNSUPPORTED Operation not supported in current configuration
*/

#if defined(ESP_NOW_TRANSMITTER) || defined (ESP_NOW_BOTH)
void esp_now_tx_callback(uint8_t * mac_address, uint8_t transmit_status)
{
  esp_now_tx_flag = true;
  Serial.println("-----ESP TX callback called-----");
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
#endif

#if defined(ESP_NOW_RECEIVER) || defined (ESP_NOW_BOTH)
void esp_now_rx_callback(uint8_t * mac_address, uint8_t * recv_data, uint8_t len)
{
  esp_now_rx_flag = true;
  Serial.println("-----ESP RX callback called-----");
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  recv_msg_len = len;
  memcpy((char*)recv_msg, (char*)recv_data, len);
}
#endif


int8_t esp_error = 0;
void esp_error_display(void)
{
  switch (esp_error)
  {
    case -1:
      Serial.println("ESP_FAIL  Generic failure (e.g., peer not found, send queue full)");
      break;
    case -2:
      Serial.println("ESP_ERR_NO_MEM  Out of memory (heap allocation failed)");
      break;
    case -3:
      Serial.println("ESP_ERR_INVALID_ARG Invalid argument passed (e.g., null pointers, wrong data length)");
      break;
    case -4:
      Serial.println("ESP_ERR_INVALID_STATE ESP-NOW not initialized or Wi-Fi not in STA/AP mode");
      break;
    case -5:
      Serial.println("ESP_ERR_NOT_FOUND Peer not found (when calling esp_now_del_peer or esp_now_send)");
      break;
    case -6:
      Serial.println("ESP_ERR_NO_PEER No peer has been added yet (send will fail)");
      break;
    case -7:
      Serial.println("ESP_ERR_INTERNAL  Internal error (unexpected behavior or corrupted control block)");
      break;
    case -8:
      Serial.println("ESP_ERR_UNSUPPORTED Operation not supported in current configuration");
      break;
  }
}
void esp_success_display(String func)
{
  Serial.print(func);
  Serial.print(" ");
  Serial.println("SUCCESS");
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN,  HIGH);
  wifi_station_init(); // WiFi access point initialization
  esp_now_initialization(); // ESP NOW initialization
}

void loop() {
  // put your main code here, to run repeatedly:

  if (esp_now_tx_flag == true)
  {
    esp_now_tx_flag = false;
    Serial.println("ESP NOW DATA Transmitted successfully");
  }
  if (esp_now_rx_flag == true)
  {
    esp_now_rx_flag = false;
    Serial.println("ESP NOW DATA Received successfully");
    Serial.println("Received Message on Receiver side : ");
    Serial.println(recv_msg);
    delay(2000);
    char *ch = strchr(recv_msg, '!');
    if (ch != NULL)
    {
      hand_shaken_cnt = atoi((ch + 1));
      uint8_t len = strlen(msg_content);
      memcpy((char*)send_msg, (char*)msg_content, len);
      if (++hand_shaken_cnt == 129)
      {
        hand_shaken_cnt = 0;
      }
      int size = snprintf((char*)&send_msg[len], sizeof(send_msg) - len, "%d", hand_shaken_cnt);
      Serial.println("Transmitting Message on Receiver side: ");
      Serial.println(send_msg);
      esp_error = esp_now_send((uint8_t*)TRANSMITTER_MAC_ADDRESS, (uint8_t*)send_msg, size + len + 1);
      (esp_error == 0) ? esp_success_display("SEND_DATA") : esp_error_display();
    }
  }
//  delay(5000);
  /********************ESP NOW Transmit Data********************/
}

void wifi_station_init(void)
{
  Serial.println(" ");
  Serial.println("*****WIFI - STATION MODE*****");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void esp_now_initialization(void)
{
  Serial.println("ESP COMBO - RECEIVER");
  delay(1000);
  /********************ESP NOW initialization********************/
  uint8_t esp_now_fail_cnt = 0;
  while (esp_now_fail_cnt < 5)
  {
    if (esp_now_init() == 0)
    {
      Serial.println("ESP NOW initialized successfully");
      break;
    }
    esp_now_fail_cnt++;
    if (esp_now_fail_cnt == 5)
    {
      Serial.println("ESP NOW initialization failure");
    }
  }


#ifdef ESP_NOW_TRANSMITTER
  /********************ESP NOW Role setting & Peer Addition********************/
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_error = esp_now_add_peer((uint8_t*)TRANSMITTER_MAC_ADDRESS, ESP_NOW_ROLE_CONTROLLER, 0, NULL, 0);
  (esp_error == 0) ? esp_success_display("PEER_ADDITION") : esp_error_display();
  /********************ESP NOW Send finished callback function registration********************/
  esp_error = esp_now_register_send_cb(esp_now_tx_callback);
  (esp_error == 0) ? esp_success_display("SEND_CALLBACK") : esp_error_display();

#elif ESP_NOW_RECEIVER
  /********************ESP NOW Role setting********************/
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  /********************ESP NOW receive finished callback function registration********************/
  esp_error = esp_now_register_recv_cb(esp_now_rx_callback);
  (esp_error == 0) ? esp_success_display("RECEIVE_CALLBACK") : esp_error_display();

#elif ESP_NOW_BOTH
  /********************ESP NOW Role setting & Peer Addition********************/
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_error = esp_now_add_peer((uint8_t*)TRANSMITTER_MAC_ADDRESS, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
  (esp_error == 0) ? esp_success_display("PEER_ADDITION") : esp_error_display();

  /********************ESP NOW both send finished & receive finished callback function registration********************/
  esp_error = esp_now_register_send_cb(esp_now_tx_callback);
  (esp_error == 0) ? esp_success_display("SEND_CALLBACK") : esp_error_display();
  esp_error = esp_now_register_recv_cb(esp_now_rx_callback);
  (esp_error == 0) ? esp_success_display("RECEIVE_CALLBACK") : esp_error_display();
#endif

}
