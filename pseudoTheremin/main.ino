#define TRIG 10
#define ECHO 11

// #define IR_TELEM A1

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  pinMode(ECHO, INPUT);
}

// In cm
int getDistance() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH)/58;
}

void loop() {
  Serial.println(getDistance());
  //Serial.println(String("From IR: ") + analogRead(IR_TELEM));
  delay(100);
}
