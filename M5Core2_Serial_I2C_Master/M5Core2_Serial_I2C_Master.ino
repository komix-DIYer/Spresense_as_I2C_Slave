#include <M5Core2.h>
#include <WireSlaveRequest.h>

#define SDA_PIN 32
#define SCL_PIN 33
#define I2C_ADDR 0x04
#define MAX_SLAVE_RESPONSE_LENGTH 2

uint8_t val[2] = {0, 0};

void setup()
{
  M5.begin(); // (true, true, true, false); // LCD, SD, Serial, I2C
  //Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  //M5.Lcd.setTextColor(M5.Lcd.color565(255, 255, 255));
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void loop()
{
  uint8_t i = 0, id = 0, va = 0;
  
  WireSlaveRequest slaveReq(Wire, I2C_ADDR, MAX_SLAVE_RESPONSE_LENGTH);
  slaveReq.setRetryDelay(5);
  if (slaveReq.request())
  {
    while (0 < slaveReq.available())
    {
      val[i] = slaveReq.read();
      Serial.print(val[i]);
      Serial.print(" ");
      i++;
    }
    Serial.println();
  }
  else
  {
    Serial.println(slaveReq.lastStatusToString());
  }

  id = val[0];
  va = val[1];

//  Wire.beginTransmission(I2C_ADDR);
//  Wire.endTransmission();
//  Wire.requestFrom(I2C_ADDR, 2);
//  if ( Wire.available() ) id = Wire.read();
//  if ( Wire.available() ) va = Wire.read();
//  
//  Serial.print(id);
//  Serial.print(", ");
//  Serial.println(va);

  M5.Lcd.setCursor(2, 2);
  M5.Lcd.print(id);
  M5.Lcd.print(", ");
  M5.Lcd.println(va);
  
  delay(999);
}
