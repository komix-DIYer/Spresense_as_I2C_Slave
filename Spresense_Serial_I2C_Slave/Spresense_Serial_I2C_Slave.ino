uint8_t v[2]={0, 0};

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1);
}

void loop()
{
  Serial.print(v[0]);  Serial.print(",");  Serial.print(v[1]);  Serial.println();
  Serial2.print(v[0]); Serial2.print(","); Serial2.print(v[1]); Serial2.println();

  v[0] = v[0] + 1;
  v[1] = v[1] + 10;

  if (v[0] > 100) v[0] = 0;
  if (v[1] > 255) v[1] = 0;
  
  delay(999);
}
