#define DIAG 1
// 
// IDE Integared Development Enviroment
// URL Uniform Resource Locator
// framework :  insieme di moduli software per facilitare lo sviluppo di applicativi,
//              nel caso di sistemi embedded, ha anche il compito di astrarre l'hardware.
//
// SETUP AMBIENTE DI SVILUPPO IDE ARDUINO
// Per poter usare una scheda con l'IDE di arduino, deve esistere il corrispondente
//              framework : es. Arduino Uno, Arduino Mega, STM32, ESP32.
// Il framework per le schede Arduino viene installato automaticamente insieme all'IDE,
//              mentre per le altre, si installa il framework manualmente.
// Per usare la scheda ESP32 C3 (IDE 2.3.5) : 
//              - nel menù File->Preferenze->URL aggiuntive per Gestore schede aggiungere il link :
//                https://espressif.github.io/arduino-esp32/package_esp32_index.json
//                se si usano + URL separare i link con la virgola.
//              - nel menù Strumenti->Scheda->Gestore Schede (oppure icona scheda sulla sinistra) :
//                inserire nella finestrella di ricerca ESP32, comparirà 'esp32 da Espressif Systems'.
//                A questo punto, selezionare la versione da installare nel nostro caso 3.2.1 e premere
//                INSTALLA oppure AGGIORNAMENTO se la versione da installare e' diversa da quella gia' installata.
//                L'operazione puo' richiedere un po' di tempo, aspettare fino a quando compare 3.2.1 installato.
//
//  Sistema di monitoraggio umidità dei "punti verde", all'interno alla biblioteca
//  Funzionamento
//    Ogni punto verde ha una apparecchiatura, che misura l'umidità del terreno.
//    Ciclicamnete, ad esempio 3h, trasmette al monitor il valore misurato.
//    La pressione del pulsante 'manutenzione' (reed), provoca la sveglia del processore e mette l'apparecchiatura in ascolto 
//    sulla seriale, in modo da ricevere dei comandi per regolare i parametri.
//  Parametri
//    - frequenza di lettura
//    - id del punto
//  Comandi       
//    - leggi parametri 
//    - cambia parametro
// 
//  
//
//  HARDWARE punto verde
//    - ESP32 C3 super mini con antenna esterna
//    - sensore umidita' capacitivo con uscita analogica 0->3V
//    - batteria ricaricabile litio 1000mA/h 
//    - carica batteria wireless TX uno per tutti i nodi
//    - carica batteria wireless RX
//    - modulo con BMS per gestione batteria
//    - reed per cambiare modalità con magnete
//    - resistenze per il partitore misura carica batteria
//  
//  HARDWARE monitor
//    - CYD ESP32 : visualizzatore con ESP32 integrato
//
//
// Con esp32 c3 super mini, in deep sleep si possono usare solo i GPIO 0 1 3 4 5
// (nella modalità Light Sleep si possono usare tutti ma il consumo è maggiore).
//
// NOTE sulla conversione anologico digitale 
//    La scheda che usiamo ha solo 2 ingressi analogici che appartengono allo stesso gruppo : ADC1,
//    quindi gli ingressi inevitabilmente avranno le stesse caratteristiche configurate.
//    Il limite di misura è 3v, che va bene per il sensore di umidità ma non per la batteria.
//    Batteria agli ioni di litio da 3.7V : carica massima 4,2V, completamente scarica 3.2V 
//
// NOTE alimentazione esp32 c3 super mini : 
//    - Ogni punto verde e' alimentato a batteria. L'obbiettivo e' quello di consumare meno energia possibile. 
//    - Fondamentale : !!attenzione!! Quando si usa un'alimentazione esterna NON collegare il cavo usb standard si danneggia la scheda esp.
//      Per evitare danneggiamenti ho usato una prolunga USB maschio-femmina e ho tagliato il filo +5V. Quindi il collegamento è :
//      [ESP32C3]-USBCfemmina<-USBCmaschio-[cavo standard]-USBmaschio->USBfemmina-[prolunga modificata]-USBmaschio->USBfemmina-[PC]. 
//    - Il pin +5v accetta da 3.3v a 6v, il problema e' che il regolatore consuma qualche mA, anche in deep sleep.
//      il pin +3v3 accetta da 3.0v a 3.6v ma la batteria a piena carica e' 4.2v. Il compromesso che ho adottato e'
//      quello di alimentare collegando il pin 3.3v, che non ha il regolatore, e mettere in serie all'alimentazione 
//      un diodo silicio 1N4xxx, per abbassare di 0.6v la tensione di batteria. Il limite per esp32c3 2.7v minimo, quindi la batteria
//      non puo scendere sotto i 3.3v (20%). In questa configurazione in deep sleep consuma 10uA.
//
//      caricaBatteria TX (removibile) -> scheda regolazione carica -> batteria 
//                                                          |-----> diodo -> esp32c3 3v3     
//
//

// CONNESSIONI:
//
//                           USB 
//                      ____|  |____
//             GPIO5   |            |-+5v   
//             GPIO6   |            |-Gnd   ---------------- scheda gestione Batteria OUT-   
//             GPIO7   |            |-+3.3v ------<diodo---- scheda gestione Batteria OUT+
//             GPIO8   |            |-GPIO4 ---------------- output alimentazione sensore umidita'
//             GPIO9   |            |-GPIO3 ---------------- input analogico misura batteria carica
//             GPIO10  |            |-GPIO2 ---------------- output on/off partitore di misura livello batteria
//             GPIO20  |            |-GPIO1 ---------------- input pulsante manutenzione 
//             GPIO21  |____________|-GPIO0 ---------------- input analogico misura sensore umidita' 
//
//
//            collegamento per la misura della batteria
//
//                                    47K         47K
//             + batteria -------------R1----|-----R2-------- GPIO2
//                                           |
//                                           |--------------- GPIO3
//    
//
// maschera individuazione stato dei GPIO
// 0x1    = GPIO_NUM_0   Input analogica sensore  ADC1_0
// 0x2    = GPIO_NUM_1   Input ingresso pulsante manutenzione
// 0x4    = GPIO_NUM_2   Output per circolazione corrente nel partitore misura batteria
// 0x8    = GPIO_NUM_3   Input analogica misura batteria
// 0x10   = GPIO_NUM_4   Output alimentazione sensore umidita'   
// 0x20   = GPIO_NUM_5   Output attivazione circuito di misura stato batteria 
//
//
// Misure
// Tolto il led rosso della scheda, in deep sleep dalla batteria circolano 120uA senza il sensore collegato.
// Durata batteria circa 9 mesi  con ciclo 6h 
//
// Per la comunicazione NODI -> MONITOR
// Si usa un router isolato, ogni nodo si connette al wifi del router con IP statico 192.168.1.xx
// dove xx e' l'id del nodo 1-10. Il monitor e' un server UDP, per i dettagli vedi GreenAlertCommon.h e DoManu.h
//
//  
//

#include <WiFi.h>

#include <Preferences.h>

#include <GreenAlertCommon.h>

#include "esp_sleep.h"

#include "DoManu.h"


#define PIN_PULS_MANUTENZIONE  GPIO_NUM_1
#define maskPinManutenzione  0x02ULL

// Timer deep sleep in microsecondi
uint64_t DEEP_SLEEP_US = 60ULL * 1000000ULL;  // 60 secondi ULL sta per unsigned long long   3600 = 1h    10800 = 3h

// tick al boot 
unsigned long  bootTick;


// frame da trasmettere
TxFrame txFrame;

// dati per il monitor dei punti verde
DataNode *txDati = &txFrame.data;	


volatile bool txDone = false;
volatile bool txOk = false;

uint8_t retryCount = 0;
const uint8_t MAX_RETRY = 3;


// Lettura livello batteria
// partitore (R1+R2)/R2   (47K+47K)/47K = 2 
int LivelloBatteria()
{
  // provoca la circolazione della corrente nel partitore
  pinMode(GPIO_NUM_2, OUTPUT);
  digitalWrite(GPIO_NUM_2, LOW);
  delay(20);

  // media su 5 letture 
  int adc = analogRead(GPIO_NUM_3); // butta la prima lettura
  adc = analogRead(GPIO_NUM_3);
  adc += analogRead(GPIO_NUM_3);
  adc += analogRead(GPIO_NUM_3);
  adc += analogRead(GPIO_NUM_3);
  adc += analogRead(GPIO_NUM_3);
  adc /= 5;

  // alta impedenza  
  pinMode(GPIO_NUM_2, INPUT);
 
  // 4.09/2780 = 0.001471223
  double vbat = adc * 0.001471223;

#ifdef DIAG
  Serial.print(adc);
  Serial.print("  ");

  Serial.print(vbat);
  Serial.print("  ");
#endif

  if(vbat >= 4.1) return 100;
  if(vbat >= 4.0) return 90;
  if(vbat >= 3.9) return 80;
  if(vbat >= 3.8) return 70;
  if(vbat >= 3.7) return 60;
  if(vbat >= 3.6) return 50;
  if(vbat >= 3.5) return 40;
  if(vbat >= 3.4) return 30;    // allarme ricaricare batteria (metterlo nei parametri)
  if(vbat >= 3.3) return 20;
  if(vbat >= 3.2) return 10;
  if(vbat >= 3.1) return 5;

  return 0;
}

// Lettura livello umidita'
int LivelloUmidita()
{
  pinMode(GPIO_NUM_4, OUTPUT);
  digitalWrite(GPIO_NUM_4, HIGH);
  delay(300);
  float adc = (float)analogRead(GPIO_NUM_0);
  adc += (float)analogRead(GPIO_NUM_0);
  adc += (float)analogRead(GPIO_NUM_0);
  adc += (float)analogRead(GPIO_NUM_0);
  adc += (float)analogRead(GPIO_NUM_0);
  adc /= 5;
  int percento;
  percento = map((long)adc, 3419, 1100, 0, 100);
  if(percento > 100)
    percento = 100;
  if(percento < 0)
    percento = 0;
#ifdef DIAG  
  Serial.println(adc);
#endif
  digitalWrite(GPIO_NUM_4, LOW);
  return percento;
}

// connessione al monitor
bool doConnect(IPAddress &IP, uint32_t timeout)
{
  // Configurazione IP statico (evita DHCP e velocizza connessione)
  if(!WiFi.config(IP, gateway, subnet)) {
    ;
    #ifdef DIAG
    Serial.println("doTx:Errore nella configurazione IP");
    #endif
  }

  // Avvio connessione WiFi
  WiFi.begin(ssid, password);

  #ifdef DIAG
  Serial.print("Connessione a WiFi");
  #endif
  unsigned long t0 = millis();
  while(WiFi.status() != WL_CONNECTED && millis() - t0 < timeout) {
    #ifdef DIAG
    Serial.print(".");
    #endif
    delay(500);
  }
  #ifdef DIAG
  Serial.println();
  #endif

  if(WiFi.status() == WL_CONNECTED) {
    #ifdef DIAG
    Serial.println("Connesso al WiFi!");
    Serial.print("IP locale: ");
    Serial.println(WiFi.localIP());
    #endif
    return true;
  }

  #ifdef DIAG
  Serial.println("Impossibile connettersi al WiFi.");
  #endif
  return false;
}

WiFiClient client;
WiFiUDP udp;
uint8_t sendData(uint8_t *data, int len)
{
  if(WiFi.status() != WL_CONNECTED)
    return false;

  udp.begin(4001);   // porta locale casuale

  udp.beginPacket(serverIP, UDP_PORT);
  udp.write(data, len);
  udp.endPacket();

  // attesa ACK (max 300ms)
  unsigned long start = millis();

  while(millis() - start < 1000) {
    int packetSize = udp.parsePacket();
    if(packetSize) {
      uint8_t ack = udp.read();
      udp.stop();
      return ack;
    }
  }

  udp.stop();
  return ER_TX_TOUT;


  if(!client.connect(serverIP, 5000)) {
    client.stop();
    Serial.println("Connessione al server fallita");
    return false;
  }

  Serial.println("Server connesso");
  client.setNoDelay(true);

  size_t sent = client.write(data, len);
  if(sent != len) { 
    client.stop();
    Serial.println("client.write : trasmissione fallita");
    return false;
  }  

  // aspetta ACK
  unsigned long t0 = millis();
  while(!client.available()) {
    if(millis() - t0 > 1000) { 
      client.stop();
      Serial.println("client.available : Timeout ");
      return false;  // timeout 1000ms
    }  
  }

  uint8_t ack = client.read();
  Serial.print("ack:");
  Serial.println(ack);
  
  client.stop();
  return (ack == 0x06);   // ACK
}


bool doTxdata()
{
  // IP per il nodo = baseNodeIP + id del nodo
  IPAddress node_IP = baseNodeIP; //(192, 168, 1, 100 + paramSetup.nodeId);
  uint8_t ipId = node_IP[3] + paramSetup.nodeId;
  node_IP[3] = ipId;

  // prepara il frame
  txFrame.lenMsg = sizeof(DataNode); 
  txFrame.crc = crc16_modbus((uint8_t *)txDati, txFrame.lenMsg);

  delay(50);

  // Modalità Station
  WiFi.mode(WIFI_STA);
 
  // potenza trasmissione alta (84 = 20db)
  esp_wifi_set_max_tx_power(84);

  if(!doConnect(node_IP, 6000)) {
    Serial.println("connessione alla rete locale fallita");
    return false;
  }

  int ii, tentativi = 5;
  uint8_t answ;
  for(ii = 0; ii < tentativi; ii++) {
    answ = sendData((uint8_t *)&txFrame, sizeof(txFrame));
    switch(answ) {
      case ER_SZ_FRAME: Serial.println("ER_SZ_FRAME"); break;
      case ER_SZ_NODE:  Serial.println("ER_SZ_NODE"); break;
      case ER_CRC_NODE: Serial.println("ER_CRC_NODE"); break;
      case ER_PWD_NODE: Serial.println("ER_PWD_NODE"); break;
      case ER_ID_NODE:  Serial.println("ER_ID_NODE"); break;
      case ER_TX_TOUT:  Serial.println("ER_TX_TOUT"); break;
    }

    // tx ok    
    if(answ == TX_NODE_OK) 
      break;

    // riprova
    delay(random(100, 1000));  
  }

  // Spegne completamente WiFi per ridurre consumo
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  return (ii < tentativi);
}

void doTxNode()
{
  //return;

  // preparazione dati da trasmettere
  txDati->nodeId = paramSetup.nodeId;
  txDati->pasword = GREEN_MONITOR_PASSWORD;
  memcpy(txDati->description, paramSetup.description, sizeof(txDati->description));
  txDati->updateFreq = paramSetup.updateFreq;
  txDati->umiAlert = paramSetup.umiAlert;             // % soglia segnalazione umidita' insufficiente
  txDati->batAlert = paramSetup.batAlert;             // % soglia segnalazione batteria da ricaricare

  // misura livello batteria
  txDati->batlevel = LivelloBatteria();
#ifdef DIAG  
  Serial.print("misura analogica batteria : ");
  Serial.println(txDati->batlevel);
#endif

  // misura umidita'  terreno
  txDati->umidita = LivelloUmidita(); 
#ifdef DIAG
  Serial.print("misura analogica sensore : ");
  Serial.println(txDati->umidita);
#endif

  // tx dati
  if(doTxdata() == false) {
    ;
  #ifdef DIAG
    Serial.println("trasmissione Fallita");
  #endif
  }

  return;
}

// gestione del risveglio da pulsante o ciclico
//void doManu(uint64_t wakeMask, esp_sleep_wakeup_cause_t wakeup_reason);
void do_wakeup_reason() 
{
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  if(wakeup_reason == ESP_SLEEP_WAKEUP_GPIO) {
    // da gpio
    uint64_t wakeup_pin_mask = esp_sleep_get_gpio_wakeup_status();
    if(wakeup_pin_mask & (1ULL << 1)) {
#ifdef DIAG      
      printf("Il wakeup è stato causato da GPIO1!\n");
#endif
      doManu();
    } else {
#ifdef DIAG
      printf("Wakeup GPIO ma non da GPIO1\n");
#endif
    }

  }

  // solo se e' un risveglio per il timer continua per la trasmissione
  if(wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
#ifdef DIAG    
    Serial.println("Il wakeup è stato causato dal timer");
#endif
    doTxNode();
  }  
  
  return;   
}



void setup() 
{
  randomSeed(esp_random());
  btStop();
  bootTick = millis();

  if(!loadConfig(&paramSetup)) {
    // nodo NON configurato, entra in modalita' manuale
    doManu();
    return;
  }  
  
  // temporaneamente seriale attiva per debug
#ifdef DIAG  
  Serial.begin(115200);                      
  delay(1000);                                
#endif

#if 0
  // per tstare la modalita' manuale
  doManu();
  return;
#endif

  // ingresso basso pulsante non premuto
  pinMode(PIN_PULS_MANUTENZIONE, INPUT_PULLDOWN);  // when the pin does not have external pull-up resistor

  // analizza ed esegue il risveglio                  
  do_wakeup_reason();

  // impostazione del pin di manutenzione : provochera un risveglio quando il pin PIN_PULS_MANUTENZIONE passera da basso ad alto  0->1
  esp_deep_sleep_enable_gpio_wakeup((uint64_t)1 << PIN_PULS_MANUTENZIONE, ESP_GPIO_WAKEUP_GPIO_HIGH);

  // setup risveglio ciclico : paramSetup.updateFreq e' in minuti, viene convertito in us 
  DEEP_SLEEP_US = ((uint64_t)paramSetup.updateFreq) * 60ULL * 1000000ULL;  
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_US);


  // durata del risveglio : per valutare il consumo batteria 
  bootTick = millis() - bootTick;
  Serial.begin(115200);                      
  delay(1000);  
  Serial.print("ms:");
  Serial.println(bootTick);
  Serial.flush();

  // passaggio in deep sleep
  esp_deep_sleep_start();

  // questo return non verra mai eseguito
  return;
}


// questa funzione non verra' eseguita
void loop() {}

