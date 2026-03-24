
// se non si riesce a connettere con il browser 192.168.1.1 con windows 
//apri cmd, opzionale fai clic destro e scegli "Esegui come amministratore".
//Digita ipconfig /release, premi Invio. prova a vedere se funziona
//Digita ipconfig /renew, premi Invio.    prova a vedere se funziona 
//Digita ipconfig /flushdns, premi Invio. prova a vedere se funziona
//Digita netsh winsock reset, premi Invio. prova a vedere se funziona
//Digita netsh int ip reset, premi Invio. prova a vedere se funziona
//oppure Riavvia il PC dopo questi comandi. 

// tratto da  https://randomnerdtutorials.com/esp32-ota-elegantota-arduino/
// per aggiornare il firmware :
// - compilare con Sketch->Esporta sketch compilato
// - aggiornare firmware con 192.168.1.61/update  prendere il file da arduino/xxxxx/build/esp32xxxxx/xxx.ini.bin
//
// tratto da  https://randomnerdtutorials.com/esp32-webserial-library/
// per seriale con web 192.168.1.61/webserial
//

#include "esp_wifi.h"
#include <Arduino.h>
#include <AsyncTCP.h>             // v.3.3.6
#include <ESPAsyncWebServer.h>    // v.3.9.3
#include <ElegantOTA.h>           // v.3.1.6
#include <WebSerial.h>            // v.1.1.0

// parametri wifi in modalita' manuale
//const char* ssid = "BIBLIOLAB";
//const char* password = "bibliolabwifilocal";
///const int wifiChannel = 11; // di solito e' 6 quindi per non interferire con altri router 2.4 Ghz lo evito
const char* hostName = "PuntoVerde";

// riferimenti esterni
int LivelloBatteria();
int LivelloUmidita();
void doTxNode();

// dati di configurazione
typedef struct Config_tag {
  uint32_t        nodeId;               // valore consentito 1-10
  char            description[41];      // es. cucina, area ristoro
  uint32_t        updateFreq;           // frequenza di aggiornamento (minuti)
  uint8_t         umiAlert;             // % soglia segnalazione umidita' insufficiente
  uint8_t         batAlert;             // % soglia segnalazione batteria scarica
  bool            configured;           // true se almeno una 
} Config;

// parametri letti da flash
Config paramSetup = {0};

// parametri temporaneamente modificati prima di essere scritti in flash
Config paramTemp = {0};

// letti per verifiche durante le operazioni manuali
int    umiditaManu = 0;              // % umidita'
int    batteriaManu = 0;             // % batteria 


Preferences prefs;
void saveConfig(Config *cfg) 
{
  prefs.begin("config", false);  // Write
  prefs.putUInt("nodeId", cfg->nodeId);
  prefs.putString("description", cfg->description);
  prefs.putUInt("updateFreq", (unsigned int)cfg->updateFreq);
  prefs.putInt("umiAlert", (int)cfg->umiAlert);
  prefs.putInt("batAlert", (int)cfg->batAlert);
  prefs.putBool("configured", true);
  prefs.end();
}

extern uint64_t DEEP_SLEEP_US;

// al boot carica i dati dalla flash
bool loadConfig(Config *cfg) 
{
  prefs.begin("config", true);  // READ ONLY
  
  if(!prefs.getBool("configured", false)) {
    prefs.end();
    return false;
  }

  cfg->updateFreq = prefs.getUInt("updateFreq", 180);  // default 180 min
  cfg->nodeId = prefs.getUInt("nodeId", 0);
  prefs.getString("description", cfg->description, sizeof(cfg->description));
  cfg->umiAlert = prefs.getInt("umiAlert", 50);
  cfg->batAlert = prefs.getInt("batAlert", 50);

  cfg->configured = prefs.getBool("configured", false);
  prefs.end();


  if(cfg->nodeId < MIN_ID_NODE || cfg->nodeId > MAX_ID_NODE) {
    cfg->nodeId = MAX_ID_NODE;
  }  

  if(cfg->updateFreq < MIN_UPDATE_TIME) {
    cfg->updateFreq = MIN_UPDATE_TIME;
  }  

  if(cfg->updateFreq > MAX_UPDATE_TIME) {
    cfg->updateFreq = MAX_UPDATE_TIME;
  }
  
  if(cfg->umiAlert < ALERT_LEVEL_BAT_MIN) {
    cfg->umiAlert = ALERT_LEVEL_BAT_MIN;
  }  

  if(cfg->umiAlert < ALERT_LEVEL_SOIL_MIN) {
    cfg->umiAlert = ALERT_LEVEL_SOIL_MIN;
  }  

  return true;
}


AsyncWebServer server(80);


uint32_t parseInt(char *input) {
  return atoi(input);
}

// frame da trasmettere
extern TxFrame txFrame;

// dati per il monitor dei punti verde
extern DataNode *txDati;	

extern uint8_t sendData(uint8_t *dati, int len);

void testTx()
{
  txDati->nodeId = paramSetup.nodeId;
  txDati->pasword = GREEN_MONITOR_PASSWORD;
  memcpy(txDati->description, paramSetup.description, sizeof(txDati->description));
  txDati->updateFreq = paramSetup.updateFreq;
  txDati->umiAlert = paramSetup.umiAlert;             // % soglia segnalazione umidita' insufficiente
  txDati->batAlert = paramSetup.batAlert;             // % soglia segnalazione batteria scarica
  txDati->batlevel = LivelloBatteria();
  txDati->umidita = LivelloUmidita(); 

  // prepara il frame
  txFrame.lenMsg = sizeof(DataNode); 
  txFrame.crc = crc16_modbus((uint8_t *)txDati, txFrame.lenMsg);

  uint8_t answ;
  answ = sendData((uint8_t *)&txFrame, sizeof(txFrame));
  switch(answ) {
    case ER_SZ_FRAME: WebSerial.println("ER_SZ_FRAME"); break;
    case ER_SZ_NODE:  WebSerial.println("ER_SZ_NODE"); break;
    case ER_CRC_NODE: WebSerial.println("ER_CRC_NODE"); break;
    case ER_PWD_NODE: WebSerial.println("ER_PWD_NODE"); break;
    case ER_ID_NODE:  WebSerial.println("ER_ID_NODE"); break;
    case ER_TX_TOUT:  WebSerial.println("ER_TX_TOUT"); break;
    case TX_NODE_OK:  WebSerial.println("TX_NODE_OK"); break;
  }

  return;
}

// ricezione messaggio webserial
#define MAX_MSG_LEN 50
void recvMsg(uint8_t *data, size_t len)
{
  // dimensione del messaggio compatibile
  if(len > MAX_MSG_LEN) {
    WebSerial.print((int)len);
    WebSerial.print(":");
    WebSerial.println("Messaggio troppo lungo, massimo consentito 41 caratteri");
  }

  //WebSerial.println("Received Data...");
  String d = "";
  data[len] = '\0';
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }

  // interpretazione dei comandi
  switch(data[0]) {
    case 'I' : // id es. I25
      WebSerial.println("I ********************");
      paramTemp.nodeId = parseInt((char *)&data[1]);
    break;

    case 'D' : // descrizione max 40 es. DArea ristoro  
      WebSerial.println("D ********************");
      if(len > sizeof(paramSetup.description))
        len = sizeof(paramSetup.description);  
      strncpy(paramTemp.description, (char *)&data[1], len);
    break;
 
    case 'F' : // frequenza es. F10 sono 10 minuti
      WebSerial.println("F ********************");
      paramTemp.updateFreq = parseInt((char *)&data[1]);
      if(paramTemp.updateFreq < 1)
        paramTemp.updateFreq = 1;
    break;

    case 'U' : // % allarme livello umidita'   
      WebSerial.println("u ********************");
      paramTemp.umiAlert = parseInt((char *)&data[1]);
      if(paramTemp.umiAlert < ALERT_LEVEL_SOIL_MIN)
        paramTemp.umiAlert = ALERT_LEVEL_SOIL_MIN ;
    break;

    case 'B' : // % allarme livello batteria   
      WebSerial.println("B ********************");
      paramTemp.batAlert = parseInt((char *)&data[1]);
      if(paramTemp.batAlert < ALERT_LEVEL_BAT_MIN)
        paramTemp.batAlert = ALERT_LEVEL_BAT_MIN ;
    break;

    case 'S' : // salva i parametri in memoria persistente 
      WebSerial.println("S ********************");
      saveConfig(&paramTemp);
    break;

    case 'O' : // stampa i parametri da flash
      loadConfig(&paramSetup);
      WebSerial.println("O ********************");
      WebSerial.print("Id:"); WebSerial.println(paramSetup.nodeId);
      WebSerial.print("Descrizione:"); WebSerial.println(paramSetup.description);
      WebSerial.print("Frequenza:"); WebSerial.println(paramSetup.updateFreq);
      WebSerial.print("Alert Umidita':"); WebSerial.println(paramSetup.umiAlert);
      WebSerial.print("Alert Batteria:"); WebSerial.println(paramSetup.batAlert);
      WebSerial.print("Configurato:"); WebSerial.println(paramSetup.configured);
    break;

    case 'P' : // stampa parametri attuali in ram
      WebSerial.println("P ********************");
      //WebSerial.printf("Nome:%s, Frequenza lettura = %d,  Id punto verde = %d\n", nome, frequenzaLettura, idPuntoVerde);    
      WebSerial.print("Id:"); WebSerial.println(paramTemp.nodeId);
      WebSerial.print("Descrizione:"); WebSerial.println(paramTemp.description);
      WebSerial.print("Frequenza:"); WebSerial.println(paramTemp.updateFreq);
      WebSerial.print("Alert Umidita':"); WebSerial.println(paramSetup.umiAlert);
      WebSerial.print("Alert Batteria:"); WebSerial.println(paramSetup.batAlert);
      WebSerial.print("Configurato:"); WebSerial.println(paramTemp.configured);
    break;

    case 'V' : // stampa valori Umidita' e Batteria
      WebSerial.println("V ********************");
      umiditaManu = LivelloUmidita();
      batteriaManu = LivelloBatteria();

      //WebSerial.printf("Nome:%s, Frequenza lettura = %d,  Id punto verde = %d\n", nome, frequenzaLettura, idPuntoVerde);    
      WebSerial.print("Umidita':"); WebSerial.println(umiditaManu);
      WebSerial.print("Batteria:"); WebSerial.println(batteriaManu);
    break;

    case 'R' : // riavvio per la madalita' automatica
      WebSerial.println("R ********************");
      ESP.restart();
    break;

    case 'T' : // test trasmissione del pacchetto
      testTx();
    break;
    
    default:
      WebSerial.print("Non Interpretabile : "); WebSerial.println((char *)data);
      WebSerial.println("**** Comandi Consentiti ****");
      WebSerial.println("  IXX id nodo 1-10");
      WebSerial.println("  DXX descrizione nodo max 40");
      WebSerial.println("  FXX Frequenza di aggiornamento in minuti");
      WebSerial.println("  P stampa i parametri impostati");
      WebSerial.println("  O stampa i parametri in flash");
      WebSerial.println("  S salva i parametri impostati in flash");
      WebSerial.println("  V test sensori");
      WebSerial.println("  R riavvia il nodo");
      WebSerial.println("  T prova trasmissione dati");
      WebSerial.println("  UXX % livello alert umidita' min 20");
      WebSerial.println("  BXX % livello alert batteria min 30");
    break;

    }

  return;
}


unsigned long lastReconnect;
unsigned long msPrev;
extern void doloop(void);
extern bool doConnect(IPAddress &IP, uint32_t timeout);

void doManu(void) 
{
  // in modalita' manuale arriva se : i parametri in flash non sono validi oppure tramite pin di manutenzione
  // in ogni caso i parametri in flash sono quelli che verranno usati, paramTemp verra' modificato interagento l'operatore.
  paramTemp = paramSetup;

  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);

  // potenza trasmissione alta (84 = 20db)
  esp_wifi_set_max_tx_power(84);

  if(!doConnect(manualIP, 5000))
    Serial.println("Impossibile connettersi al WiFi.");

  server.begin();
  Serial.println("HTTP server started");

  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);

  ElegantOTA.begin(&server);  

  lastReconnect = millis();
  msPrev  = millis();

  while(1)
    doloop();
  return;
}



#define TICK_MANU     1000    // 1s  
#define TIMEOUT_MANU  3600    // 1h 

unsigned long counter = 0;
uint32_t misura;

void doloop(void) 
{
  // static unsigned long lastReconnect = 0;
  if(WiFi.status() != WL_CONNECTED && millis() - lastReconnect > 15000) {
    lastReconnect = millis();
    WiFi.reconnect();
  }    

  if(WiFi.status() != WL_CONNECTED && millis() - lastReconnect >= 15000) {
    Serial.println("!!! Timeout connessione WiFi : restart esp32 !!!");
    ESP.restart();
  }    

  if(WiFi.status() == WL_CONNECTED) {
    ElegantOTA.loop();
  }

  // uscita forzata dalla modalita manuale dopo 1h
  if((millis() - msPrev) > TICK_MANU) {
    msPrev = millis();
    counter++;
    if(counter > TIMEOUT_MANU)
      ESP.restart();  
  }

  return;
}


