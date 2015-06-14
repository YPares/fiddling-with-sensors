#define LED_PIN 8
#define MOTION_S_PIN 4

void setup()
{

}

void loop()
{
  bool somebody_is_there = digitalRead(MOTION_S_PIN);
  digitalWrite(LED_PIN, somebody_is_there);
  delay(10);
}
