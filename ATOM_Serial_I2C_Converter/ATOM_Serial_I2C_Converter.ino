/* 
 * I2C SlaveとしてSerial1からの入力をI2Cへ出力する（データ形式はカンマ区切りの複数のint型変数）
 * I2C SDA:26, SCL:32
 * Serial2 RX:33, TX:23
 */

//#include <M5Atom.h>
#include <FastLED.h>
#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

#define RX_PIN 33
#define TX_PIN 23
#define SDA_PIN 26
#define SCL_PIN 32
#define I2C_SLAVE_ADDR 0x04

#define SIZE_OF_ARRAY(ary) (sizeof(ary)/sizeof((ary)[0]))
#define RCV_DAT_NUM 2
int val[RCV_DAT_NUM]; // 数値がIntの場合

const int NUM_LEDS = 1;
const int LED_PIN = 27;
static CRGB leds[NUM_LEDS];

CRGB correct(CRGB c) {
  // fix RGB order :(
  return CRGB(c.g, c.r, c.b);
}

void requestEvent();

// シリアル通信の受信データdataを区切り文字delimiterで分割してresultに返す
int split(String *result, size_t resultsize, String data, char delimiter)
{
  int index = 0;
  int datalength = data.length();
  for (int i = 0; i < datalength; i++)
  {
    char tmp = data.charAt(i);
    if ( tmp == delimiter )
    {
      index++;
      if ( index > (resultsize - 1)) return -1;
    }
    else result[index] += tmp;
  }
  return (index + 1);
}

void setup()
{
  //M5.begin(); // UART:begin, I2C:begin, LED:-, -, https://qiita.com/penguinprogrammer/items/5abf3e4f9583e41e9cfa

  // Init LEDs
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  leds[0] = CRGB::Red;
  FastLED.show();

  // Begin Serial0&1
  Serial.begin(115200);
  while (!Serial);
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  while (!Serial1);
  
  leds[0] = CRGB::Yellow;
  FastLED.show();

  // Begin Wire (slave mode)
  bool res = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  if (!res) {
    Serial.println("I2C slave init failed");
    while(1) delay(100);
  }
  
  WireSlave.onRequest(requestEvent);
  Serial.printf("Slave joined I2C bus with addr #%d\n", I2C_SLAVE_ADDR);
  
  leds[0] = CRGB::Green;
  FastLED.show();
}

void loop()
{
  if (Serial1.available()) serial1Event(); // ESP32ではイベントとして呼び出されないため明示的に呼ぶ
  
  WireSlave.update();
  
  delay(1);
}

void serial1Event()
{
  String s = "";
  
  while(1)
  {
    if(Serial1.available()==0) break;
    
    char tmp = Serial1.read();
    
    if(tmp=='\0') break;
    s += String(tmp);
    
    delay(5);
  }
  
  String v[RCV_DAT_NUM];
  size_t arraysize = SIZE_OF_ARRAY(v);
  char delimiter = ',';
  int index = split(v, arraysize, s, delimiter); // 本来なら index == RCV_DAT_NUM
  for(int i=0; i<index; i++)
  {
    val[i] = v[i].toInt(); // 数値がintの場合 
    
    // Serialに転送
    Serial.print((uint8_t)val[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void requestEvent()
{
  for(int i=0; i<RCV_DAT_NUM; i++) WireSlave.write((uint8_t)val[i]);
}
