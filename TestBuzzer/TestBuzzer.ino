#include <Statistic.h>

Statistic vals;

#define SENSOR_PIN A0
#define BUZZER_PIN       3

#define SUBS_VALS        1
#define THRESH           300

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  for(int i=0; i<255; i=i+5)
  {
    //analogWrite(BUZZER_PIN, i);
    //delay(400);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void loop() {
  double v = analogRead(SENSOR_PIN);  
  vals.add(v);
  if(vals.count() >= SUBS_VALS)
  {
    Serial.println(vals.average());
    vals.clear();
  }
  delay(10);
}

