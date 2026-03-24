//
// Strumenti-> Partition Scheme : No FS 4MB (2MB APP x2)
// lvgl da kisvegabor 9.5.0
//


#include <WiFi.h>
extern "C" { 
  #include "esp_wifi.h" 
}

#include <GreenAlertCommon.h>


// ricorda di mettere questa define prima dell'inclusione lvgl.h
// nei file dove viene incluso come qui sotto
//#define LV_CONF_INCLUDE_SIMPLE
//#include <lvgl.h>
#define LV_CONF_INCLUDE_SIMPLE
#include <lvgl.h>

#include "ui.h"

#if 0
/*
!!!!!!! attenzione !!!!!!!!!
quando si generano i file con EEZ, le strutture delle immagini esportate, 
sono in C99 e non sono compatibili con c++. Si devono modificare a mano : 
es. c:\Users\Utente\Documents\Arduino\GreenAlert\EEZ\src\ui\images\ui_image_sfondo.c

da cosi'
const lv_img_dsc_t img_sfondo = {
  .header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA,
  .header.always_zero = 0,
  .header.reserved = 0,
  .header.w = 320,
  .header.h = 240,
  .data_size = 76800 * LV_IMG_PX_SIZE_ALPHA_BYTE,
  .data = img_sfondo_map,
};

a cosi'
const lv_img_dsc_t img_sfondo = { LV_IMG_CF_TRUE_COLOR, 0, 0, 320, 240, 76800 * LV_COLOR_SIZE / 8, img_sfondo_true_map };
*/
#include "..\EEZ\src\ui\ui_image_sfondo_true.c"


#include "..\EEZ\src\ui\screens.c"
#include "..\EEZ\src\ui\styles.c"
#include "..\EEZ\src\ui\ui.c"
#endif


// file conume per il Monitor.ini e PuntoVerde.ino
#include <GreenAlertCommon.h>


#include <Preferences.h>


const char* hostName = "GreenMonitor";

// Creazione server TCP sulla porta 5000
//WiFiServer server(5000);
WiFiUDP udp;
uint8_t bufferUdp[100];
bool udpStarted = false;

// per test
DataNode pool[MAX_NUM_NODE] = {
  { 1, 1234, "descr nodo1", 60, 50, 40},
  { 2, 1234, "descr nodo2", 61, 31, 11},
  { 3, 1234, "descr nodo3", 62, 32, 12},
  { 4, 1234, "descr nodo4", 63, 33, 43},
  { 5, 1234, "descr nodo5", 64, 34, 14},
  { 6, 1234, "descr nodo6", 65, 35, 45},
  { 7, 1234, "descr nodo7", 66, 36, 16},
  { 8, 1234, "descr nodo8", 67, 37, 47},
  { 9, 1234, "descr nodo9", 68, 38, 18},
  { 10, 1234, "descr nodo10", 69, 39, 49},
};            // ultimi dati ricevuti da tutti i nodi 


// al boot leggera' tutti i nodi, conterranno gli ultimi ricevuti 
void loadAllDataNode() 
{
  int ii;
  Preferences prefs;
  prefs.begin("DataNode", true);

  for(ii = 0; ii < MAX_NUM_NODE; ii++) {
    char key[10];
    snprintf(key, sizeof(key), "Node%d", ii);
    if(prefs.getBytesLength(key) == sizeof(DataNode)) 
      prefs.getBytes(key, &pool[ii], sizeof(DataNode));
  }

  prefs.end();
  return;
}


// aggiorna il nodo di posto ii, verra' chiamata quando il nodo notifica nuovi dati 
void saveDataNode(uint8_t ii) 
{
  Preferences prefs;
  prefs.begin("DataNode", false);

  char key[10];
  snprintf(key, sizeof(key), "Node%d", ii);
  prefs.putBytes(key, &pool[ii], sizeof(DataNode));
  prefs.end();
}

#define COLOR_BACKGROUND_MAIN     0x000000 // nero
#define COLOR_ER_BAT              0xff0000 // rosso     insufficienza batteria
#define COLOR_ER_SOIL             0xffff00 // giallo    insufficienza umidita'
#define COLOR_OK                  0x00ff00 // verde     tutto bene
//#define COLOR_LOST                0xffffff // bianco    nodo morto
      

extern bool flip1S;
int32_t GetLedColor(int ii)
{
  int32_t result = COLOR_OK;

  if(pool[ii].umidita < pool[ii].umiAlert)
    result = COLOR_ER_SOIL; // giallo

  if(pool[ii].batlevel < pool[ii].batAlert) {
    result = COLOR_ER_BAT; // rosso
  }

  if(result != COLOR_OK) {
    if(!flip1S)
      result = COLOR_BACKGROUND_MAIN;
  }

  return result;
}

//#if 0

// analisi dati ricevuti da un client
uint8_t OnDataRecv(uint8_t *rxData, int len) 
{
  // controllo dimensione del pacchetto ricevuto compatibile
  if(len != sizeof(TxFrame)) {
    Serial.println("Errore: dimensione dati non valida");
    return ER_SZ_FRAME;
  }

  // per trattare i bytes ricevuti in modo strutturato come DataNode
  TxFrame   *pFrame = (TxFrame *)rxData;
  DataNode  *pNode = &pFrame->data;

  // controllo lunghezza
  if(pFrame->lenMsg != sizeof(DataNode))
    return ER_SZ_NODE;

  // controllo crc
  uint16_t crc = crc16_modbus((uint8_t *)pNode, pFrame->lenMsg);
  if(crc != pFrame->crc)
    return ER_CRC_NODE;

  // controllo dati da un trasmettitore autorizzato
  if(pNode->pasword != GREEN_MONITOR_PASSWORD) {
    Serial.println("Errore: password non valida");
    return ER_PWD_NODE;
  }

  // controllo id nei limiti
  if(pNode->nodeId < MIN_ID_NODE  || pNode->nodeId > MAX_ID_NODE) {
    Serial.println("Errore: id illecito");
    return ER_ID_NODE;  
  }

  // salva i dati nel pool
  memcpy(&pool[pNode->nodeId -1], pNode, sizeof(DataNode));
  saveDataNode(pNode->nodeId);

  Serial.print("nodeId: ");
  Serial.println(pNode->nodeId);

  Serial.print("description: ");
  Serial.println(pNode->description);

  Serial.print("updateFreq: ");
  Serial.println(pNode->updateFreq);

  Serial.print("umidita: ");
  Serial.println(pNode->umidita);

  Serial.print("batteria: ");
  Serial.println(pNode->batlevel);

  Serial.print("umiAlert: ");
  Serial.println(pNode->umiAlert);

  Serial.print("batAlert: ");
  Serial.println(pNode->batAlert);


  return TX_NODE_OK;
}

// callback eventi interessanti per la gestione della comunicazione UDP
void WiFiEvent(WiFiEvent_t event)
{
  switch(event)
  {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.println("WiFi connesso!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());

      udp.begin(UDP_PORT);
      udpStarted = true;
      Serial.println("UDP server avviato");
      break;

    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      Serial.println("WiFi disconnesso! Riconnessione...");
      udp.stop();
      udpStarted = false;
      WiFi.reconnect();
      break;

    default:
      break;
  }
}

void setupServer() 
{
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);

  // potenza trasmissione alta (84 = 20db)
  esp_wifi_set_max_tx_power(84);

  // Impostazione IP statico
  WiFi.config(serverIP, gateway, subnet);

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connessione WiFi");
  
  return;

  // init
  //WiFi.mode(WIFI_OFF);
  //delay(1000);


  // Modalità Station (si collega al router)
  WiFi.mode(WIFI_STA);

  // potenza trasmissione alta (84 = 20db)
  esp_wifi_set_max_tx_power(84);

  // Impostazione IP statico
  WiFi.config(serverIP, gateway, subnet);

  // Connessione alla rete WiFi
  WiFi.begin(ssid, password);

  Serial.print("Connessione WiFi");

  // Attesa connessione
  while(WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Server connesso. IP: ");
  Serial.println(WiFi.localIP());

  // Avvio server TCP
  //server.begin();
  udp.begin(5000);
  return;
}



// display
#include <TFT_eSPI.h>

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;

static lv_color_t buf[screenWidth * screenHeight / 40]; //era 10
//static lv_color_t buf[screenWidth * 40];
//lv_color_t *buf;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp_drv);
  return;
}


void setupDisplay()
{
  
  //buf = (lv_color_t *)ps_malloc(screenWidth * screenHeight / 10 * sizeof(lv_color_t));

  // Init LVGL GUI
  lv_init();
  tft.begin();
  tft.setRotation(1);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * screenHeight / 40);
  //lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 40);

  // Init display
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  ui_init();

  return;
}

#include <SPI.h>
#include <XPT2046_Touchscreen.h>

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass mySpi = SPIClass(VSPI);
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
uint16_t touchScreenMinimumX = 200, touchScreenMaximumX = 3700, touchScreenMinimumY = 240, touchScreenMaximumY = 3800;

void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    //Some very basic auto calibration so it doesn't go out of range
    if (p.x < touchScreenMinimumX) touchScreenMinimumX = p.x;
    if (p.x > touchScreenMaximumX) touchScreenMaximumX = p.x;
    if (p.y < touchScreenMinimumY) touchScreenMinimumY = p.y;
    if (p.y > touchScreenMaximumY) touchScreenMaximumY = p.y;
    //Map this to the pixel position
    data->point.x = map(p.x, touchScreenMinimumX, touchScreenMaximumX, 1, screenWidth);  /* Touchscreen X calibration */
    data->point.y = map(p.y, touchScreenMinimumY, touchScreenMaximumY, 1, screenHeight); /* Touchscreen Y calibration */
    data->state = LV_INDEV_STATE_PR;
#if 0
    Serial.print( "Touch x " );
    Serial.print( data->point.x );
    Serial.print( " y " );
    Serial.println( data->point.y );
#endif    
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

  
//#endif

//static lv_indev_drv_t indev_drv;
lv_indev_t *indev;      //Touchscreen input device

void setupTouch() 
{
  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); /* Start second SPI bus for touchscreen */
  ts.begin(mySpi);                                                  /* Touchscreen init */
  ts.setRotation(1);                                                /* Landscape orientation */

  /*Initialize the (dummy) input device driver*/
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_t *my_indev = lv_indev_drv_register(&indev_drv);

  return;
}  

unsigned long msPrev;
uint32_t lastTick = 0;
bool flip1S = false;

void setup() {
  Serial.begin(115200);
  delay(2000);

  setupServer();
  setupDisplay();
  setupTouch(); 

  // scommentare quando arriveranno i dati
  loadAllDataNode();
  msPrev = millis();
  return;
}


uint8_t buffer[sizeof(DataNode)];
void loop() 
{
  // aggiornamento interfaccia utente
  if((millis() - msPrev) > 1000) {
    flip1S = !flip1S;
    msPrev = millis();
    pool[0].umidita++;
    if(pool[0].umidita > 100)
      pool[0].umidita = 0;
      
    ui_tick();
      //update_Data();
  }

  if(udpStarted) {
    int packetSize = udp.parsePacket();

    if(packetSize) {
      IPAddress remoteIP = udp.remoteIP();
      uint16_t remotePort = udp.remotePort();
      int len = udp.read(bufferUdp, sizeof(bufferUdp));
      Serial.println("-----");
      Serial.print("Ricevuti ");
      Serial.print(len);
      Serial.print(" byte da ");
      Serial.print(remoteIP);
      Serial.print(":");
      Serial.println(remotePort);

      udp.beginPacket(remoteIP, remotePort);
      udp.write(OnDataRecv(bufferUdp, len));
      udp.endPacket();
    }
  }




#if 0
  WiFiClient client = server.available(); // verifica se c'è un client nuovo/connesso

   if(client) {
    Serial.println("Client connesso!");
    unsigned long timeout = millis();
    while(client.connected() && millis() - timeout < 2000) {
      if(client.available() >= sizeof(buffer)) {
        int readBytes = client.read(buffer, sizeof(buffer));

        Serial.print("Ricevuti ");
        Serial.print(readBytes);
        Serial.println(" byte");

        // invia ACK
        client.write(0x06);
        client.flush();
        break;  // esci dopo aver gestito il pacchetto
      }
    }
    client.stop();  // chiude sempre
    Serial.println("Client disconnesso");
  }
#endif 

  lv_timer_handler(); 
  delay(5);

  return;   
}


int curViewNode = 0;
char curNodeIdStr[10];                  // dati ricevuti trsformati in stringa
char curUpdateFreqStr[10]; 
char curUmiditaStr[10]; 
char curBatteriaStr[10]; 

#if 0
void update_Data() 
{
  lv_obj_t * scr = lv_scr_act();
  if(scr == objects.main) {
      lv_bar_set_value(objects.main_bar_bat1, pool[0].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat2, pool[1].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat3, pool[2].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat4, pool[3].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat5, pool[4].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat6, pool[5].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat7, pool[6].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat8, pool[7].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat9, pool[8].batlevel, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_bat10, pool[9].batlevel, LV_ANIM_OFF);

      lv_bar_set_value(objects.main_bar_soil1, pool[0].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil2, pool[1].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil3, pool[2].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil4, pool[3].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil5, pool[4].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil6, pool[5].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil7, pool[6].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil8, pool[7].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil9, pool[8].umidita, LV_ANIM_OFF);
      lv_bar_set_value(objects.main_bar_soil10, pool[9].umidita, LV_ANIM_OFF);


      lv_led_set_color(objects.main_led1, lv_color_hex(GetLedColor(0)));
      lv_led_set_color(objects.main_led2, lv_color_hex(GetLedColor(1)));
      lv_led_set_color(objects.main_led3, lv_color_hex(GetLedColor(2)));
      lv_led_set_color(objects.main_led4, lv_color_hex(GetLedColor(3)));
      lv_led_set_color(objects.main_led5, lv_color_hex(GetLedColor(4)));
      lv_led_set_color(objects.main_led6, lv_color_hex(GetLedColor(5)));
      lv_led_set_color(objects.main_led7, lv_color_hex(GetLedColor(6)));
      lv_led_set_color(objects.main_led8, lv_color_hex(GetLedColor(7)));
      lv_led_set_color(objects.main_led9, lv_color_hex(GetLedColor(8)));
      lv_led_set_color(objects.main_led10, lv_color_hex(GetLedColor(9)));
  }

  if(scr == objects.page1) {
    snprintf(curNodeIdStr, sizeof(curNodeIdStr), "%d", pool[curViewNode].nodeId);
    snprintf(curUpdateFreqStr, sizeof(curUpdateFreqStr), "%d", pool[curViewNode].updateFreq);
    snprintf(curUmiditaStr, sizeof(curUmiditaStr), "%d", pool[curViewNode].umidita);
    snprintf(curBatteriaStr, sizeof(curBatteriaStr), "%d", pool[curViewNode].batlevel);
 

    lv_label_set_text(objects.desrval, pool[curViewNode].description);
    lv_label_set_text(objects.idval, curNodeIdStr);
    lv_label_set_text(objects.batval, curUpdateFreqStr);
    lv_label_set_text(objects.umival, curUmiditaStr);
    lv_label_set_text(objects.freeval, curBatteriaStr);
  }

  return;
}  
#endif

// array di oggetti creati da EEZ studio vedi file screens.h
extern objects_t objects;

// Apre pagina per il nodo ii. 
// Dopo la chiamata di questa funzione, 
// la pagina si aggiorna con i dati dell'iiesimo nodo rappresentato da curViewNode. (vedi update_Data()) 
void setupDataPage(int ii, lv_event_t * ev)
{
  lv_event_code_t code = lv_event_get_code(ev); 
  if(code != LV_EVENT_CLICKED) 
    return; 
  curViewNode = ii;
  loadScreen(SCREEN_ID_PAGE1);
  return;
}

extern "C" { 

// eventi predisposti in EEZ studio per cambiare pagina. Ciclicamente la pagina viene aggiornata  
void action_to_main_page(lv_event_t * ev)        
{
  lv_event_code_t code = lv_event_get_code(ev); 
  if(code != LV_EVENT_CLICKED) return; 
  loadScreen(SCREEN_ID_MAIN); 
  ui_tick(); 
  return; 
}
void action_on_led_clicked1(lv_event_t * ev)     {setupDataPage(0, ev);}
void action_on_led_clicked2(lv_event_t * ev)     {setupDataPage(1, ev);}
void action_on_led_clicked3(lv_event_t * ev)     {setupDataPage(2, ev);}
void action_on_led_clicked4(lv_event_t * ev)     {setupDataPage(3, ev);}
void action_on_led_clicked5(lv_event_t * ev)     {setupDataPage(4, ev);}
void action_on_led_clicked6(lv_event_t * ev)     {setupDataPage(5, ev);}
void action_on_led_clicked7(lv_event_t * ev)     {setupDataPage(6, ev);}
void action_on_led_clicked8(lv_event_t * ev)     {setupDataPage(7, ev);}
void action_on_led_clicked9(lv_event_t * ev)     {setupDataPage(8, ev);}
void action_on_led_clicked10(lv_event_t * ev)    {setupDataPage(9, ev);}


extern int32_t get_var_bar_bat1();
extern void set_var_bar_bat1(int32_t value);
extern int32_t get_var_bar_bat2();
extern void set_var_bar_bat2(int32_t value);
extern int32_t get_var_bar_bat3();
extern void set_var_bar_bat3(int32_t value);
extern int32_t get_var_bar_bat4();
extern void set_var_bar_bat4(int32_t value);
extern int32_t get_var_bar_bat5();
extern void set_var_bar_bat5(int32_t value);
extern int32_t get_var_bar_bat6();
extern void set_var_bar_bat6(int32_t value);
extern int32_t get_var_bar_bat7();
extern void set_var_bar_bat7(int32_t value);
extern int32_t get_var_bar_bat8();
extern void set_var_bar_bat8(int32_t value);
extern int32_t get_var_bar_bat9();
extern void set_var_bar_bat9(int32_t value);
extern int32_t get_var_bar_bat10();
extern void set_var_bar_bat10(int32_t value);
extern int32_t get_var_bar_soil1();
extern void set_var_bar_soil1(int32_t value);
extern int32_t get_var_bar_soil2();
extern void set_var_bar_soil2(int32_t value);
extern int32_t get_var_bar_soil3();
extern void set_var_bar_soil3(int32_t value);
extern int32_t get_var_bar_soil4();
extern void set_var_bar_soil4(int32_t value);
extern int32_t get_var_bar_soil5();
extern void set_var_bar_soil5(int32_t value);
extern int32_t get_var_bar_soil6();
extern void set_var_bar_soil6(int32_t value);
extern int32_t get_var_bar_soil7();
extern void set_var_bar_soil7(int32_t value);
extern int32_t get_var_bar_soil8();
extern void set_var_bar_soil8(int32_t value);
extern int32_t get_var_bar_soil9();
extern void set_var_bar_soil9(int32_t value);
extern int32_t get_var_bar_soil10();
extern void set_var_bar_soil10(int32_t value);
extern int32_t get_var_color_led1();
extern void set_var_color_led1(int32_t value);
extern int32_t get_var_color_led2();
extern void set_var_color_led2(int32_t value);
extern int32_t get_var_color_led3();
extern void set_var_color_led3(int32_t value);
extern int32_t get_var_color_led4();
extern void set_var_color_led4(int32_t value);
extern int32_t get_var_color_led5();
extern void set_var_color_led5(int32_t value);
extern int32_t get_var_color_led6();
extern void set_var_color_led6(int32_t value);
extern int32_t get_var_color_led7();
extern void set_var_color_led7(int32_t value);
extern int32_t get_var_color_led8();
extern void set_var_color_led8(int32_t value);
extern int32_t get_var_color_led9();
extern void set_var_color_led9(int32_t value);
extern int32_t get_var_color_led10();
extern void set_var_color_led10(int32_t value);
extern const char *get_var_node_id();
extern void set_var_node_id(const char *value);
extern const char *get_var_description();
extern void set_var_description(const char *value);
extern const char *get_var_update_freq();
extern void set_var_update_freq(const char *value);
extern const char *get_var_umidita();
extern void set_var_umidita(const char *value);
extern const char *get_var_batteria();
extern void set_var_batteria(const char *value);

// le funzioni set e get in EEZ studio vengono chiamate solo al boot 
// poi servono solo se la si usa la modalità flow che e' un casino (documentazione poca e imprecisa)
int32_t get_var_bar_bat1(){ return pool[0].batlevel; }
void set_var_bar_bat1(int32_t value){}
int32_t get_var_bar_bat2(){ return pool[1].batlevel; }
void set_var_bar_bat2(int32_t value){}
int32_t get_var_bar_bat3(){ return pool[2].batlevel; }
void set_var_bar_bat3(int32_t value){}
int32_t get_var_bar_bat4(){ return pool[3].batlevel; }
void set_var_bar_bat4(int32_t value){}
int32_t get_var_bar_bat5(){ return pool[4].batlevel; }
void set_var_bar_bat5(int32_t value){}
int32_t get_var_bar_bat6(){ return pool[5].batlevel; }
void set_var_bar_bat6(int32_t value){}
int32_t get_var_bar_bat7(){ return pool[6].batlevel; }
void set_var_bar_bat7(int32_t value){}
int32_t get_var_bar_bat8(){ return pool[7].batlevel; }
void set_var_bar_bat8(int32_t value){}
int32_t get_var_bar_bat9(){ return pool[8].batlevel; }
void set_var_bar_bat9(int32_t value){}
int32_t get_var_bar_bat10(){ return pool[9].batlevel; }
void set_var_bar_bat10(int32_t value){}
int32_t get_var_bar_soil1(){ return pool[0].umidita; }
void set_var_bar_soil1(int32_t value){}
int32_t get_var_bar_soil2(){ return pool[1].umidita; }
void set_var_bar_soil2(int32_t value){}
int32_t get_var_bar_soil3(){ return pool[2].umidita; }
void set_var_bar_soil3(int32_t value){}
int32_t get_var_bar_soil4(){ return pool[3].umidita; }
void set_var_bar_soil4(int32_t value){}
int32_t get_var_bar_soil5(){ return pool[4].umidita; }
void set_var_bar_soil5(int32_t value){}
int32_t get_var_bar_soil6(){ return pool[5].umidita; }
void set_var_bar_soil6(int32_t value){}
int32_t get_var_bar_soil7(){ return pool[6].umidita; }
void set_var_bar_soil7(int32_t value){}
int32_t get_var_bar_soil8(){ return pool[7].umidita; }
void set_var_bar_soil8(int32_t value){}
int32_t get_var_bar_soil9(){ return pool[8].umidita; }
void set_var_bar_soil9(int32_t value){}
int32_t get_var_bar_soil10(){ return pool[9].umidita; }
void set_var_bar_soil10(int32_t value){}



// variabili pagina Main
int32_t get_var_color_led1(){ return GetLedColor(0); }
void set_var_color_led1(int32_t value){}
int32_t get_var_color_led2(){ return GetLedColor(1); } 
void set_var_color_led2(int32_t value){}
int32_t get_var_color_led3(){ return GetLedColor(2); }
void set_var_color_led3(int32_t value){}
int32_t get_var_color_led4(){ return GetLedColor(3); }
void set_var_color_led4(int32_t value){}
int32_t get_var_color_led5(){ return GetLedColor(4); }
void set_var_color_led5(int32_t value){}
int32_t get_var_color_led6(){ return GetLedColor(5); }
void set_var_color_led6(int32_t value){}
int32_t get_var_color_led7(){ return GetLedColor(6); }
void set_var_color_led7(int32_t value){}
int32_t get_var_color_led8(){ return GetLedColor(7); }
void set_var_color_led8(int32_t value){}
int32_t get_var_color_led9(){ return GetLedColor(8); }
void set_var_color_led9(int32_t value){}
int32_t get_var_color_led10(){ return GetLedColor(9); }
void set_var_color_led10(int32_t value){}



const char *get_var_node_id() { snprintf(curNodeIdStr, sizeof(curNodeIdStr), "%d", pool[curViewNode].nodeId); return curNodeIdStr; }
void set_var_node_id(const char *value) {}
const char *get_var_description(){ return pool[curViewNode].description; }
void set_var_description(const char *value){}
const char *get_var_update_freq(){ snprintf(curUpdateFreqStr, sizeof(curUpdateFreqStr), "%d", pool[curViewNode].updateFreq); return curUpdateFreqStr; }
void set_var_update_freq(const char *value){}
const char *get_var_umidita(){ snprintf(curUmiditaStr, sizeof(curUmiditaStr), "%d", pool[curViewNode].umidita); return curUmiditaStr; }
void set_var_umidita(const char *value){}
const char *get_var_batteria(){ snprintf(curBatteriaStr, sizeof(curBatteriaStr), "%d", pool[curViewNode].batlevel); return curBatteriaStr; }
void set_var_batteria(const char *value){}

} // extern "C"


//#ifdef __cplusplus
//}
//endif

