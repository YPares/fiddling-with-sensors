#define THERMISTOR_B_VALUE 3975
#define DHT_PIN A0

#include <Wire.h>
#include <rgb_lcd.h>
#include <DHT.h>

rgb_lcd lcd;
DHT dht(DHT_PIN, DHT11);

#ifdef SERVO_AS_POINTER
#include <Servo.h>

#define SERVO_PIN 3

Servo servo;
#endif

void setup() 
{
    Serial.begin(9600);
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    lcd.setPWM(REG_RED, 255);
    lcd.setPWM(REG_BLUE, 255);
    lcd.setPWM(REG_GREEN, 255);

    #ifdef SERVO_AS_POINTER
    servo.attach(SERVO_PIN);
    #endif
}

/*  // Used only with the basic temperature sensor
float reading_to_temp(int val) {
   // Determine the current resistance of the thermistor based on the sensor value.
   float resistance = (float)(1023-val)*10000/val;

   // Calculate the temperature based on the resistance value.
   return 1/(log(resistance/10000)/THERMISTOR_B_VALUE+1/298.15)-273.15;
}*/

float ramp_value(float x, float min_x, float max_x, float min_y, float max_y) {
  if (x > max_x)
    x = max_x;
  if (x < min_x)
    x = min_x;
  return min_y + (x - min_x)*(max_y - min_y)/(max_x - min_x);
}

float ramp_log_value(float x, float min_x, float max_x, float min_y, float max_y) {
  return ramp_value(log(x), log(min_x), log(max_x), min_y, max_y);
  // map function can't handle reverse intervals, apparently.
}

void loop()
{
    int temperature = dht.readTemperature();
    int humidity = dht.readHumidity();
    
    lcd.setPWM(REG_BLUE, ramp_log_value(humidity, 50, 100, 0, 255/3));
    lcd.setPWM(REG_GREEN, ramp_log_value(temperature, 15, 30, 255, 0));
    lcd.clear();
    lcd.print(String("Temp: ") + temperature + ".C");
    lcd.setCursor(0,1);
    lcd.print(String("Humi: ") + humidity + "%");

    #ifdef SERVO_AS_POINTER
    servo.write(ramp_value(temperature, 10, 35, 150, 0));
    #endif
    
    delay(1000);
}
