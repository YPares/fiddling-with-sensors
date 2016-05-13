#include "copilot-c99-codegen/copilot.c"

#define GND 3
#define RED 4
#define GREEN 5
#define BLUE 6

void setup() {
  Serial.begin(9600);
  pinMode(GND, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  digitalWrite(GND, LOW);
}

void write_comps(int32_t r, int32_t g, int32_t b) {
  Serial.println(g);
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

void loop() {
  step();
  delay(10);
}
