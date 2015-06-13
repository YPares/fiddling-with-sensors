#define OLED_SCREEN 1
#define RGB_LCD_SCREEN 1

#define THERMISTOR_B_VALUE 3975
#define DHT_PIN A0

#include <Wire.h>
#include <DHT.h>

DHT dht(DHT_PIN, DHT11);

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

#ifdef RGB_LCD_SCREEN
#include <rgb_lcd.h>
namespace lcd {
  rgb_lcd screen;

  void setup() {
    screen.begin(16, 2);
    screen.setPWM(REG_RED, 255);
    screen.setPWM(REG_BLUE, 255);
    screen.setPWM(REG_GREEN, 255);
  }

  void draw(int humidity, int temperature)
  {
    screen.setPWM(REG_BLUE, ramp_log_value(humidity, 50, 100, 0, 255/3));
    screen.setPWM(REG_GREEN, ramp_log_value(temperature, 15, 30, 255, 0));
    screen.clear();
    screen.print(String("Temp: ") + temperature + ".C");
    screen.setCursor(0,1);
    screen.print(String("Humi: ") + humidity + "%");
  }
}
#endif

#ifdef OLED_SCREEN
#include <U8glib.h>
namespace oled {
  U8GLIB_SSD1306_128X64 screen(U8G_I2C_OPT_NO_ACK);
  #define u8g screen
  
  void setup() {
    screen.setColorIndex(1); // pixel on. My screen mode is U8G_MODE_BW    
    screen.setFont(u8g_font_gdr20r);
  }
  
  void draw(int humidity, int temperature) {
    screen.firstPage();
    do {
      screen.setPrintPos(0, 24);
      screen.println(String("T: ") + temperature + ".C");
      screen.setPrintPos(0, 48);
      screen.print(String("H: ") + humidity + "%");
    } while (screen.nextPage());
  }
}
#endif

#ifdef SERVO_AS_POINTER
#include <Servo.h>
#define SERVO_PIN 3
Servo servo;
#endif

void setup() 
{
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  #ifdef RGB_LCD_SCREEN
  lcd::setup();
  #endif
  #ifdef OLED_SCREEN
  oled::setup();
  #endif  
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

void loop()
{
    int humidity = dht.readHumidity();    
    int temperature = dht.readTemperature();
    
    #ifdef RGB_LCD_SCREEN
    lcd::draw(humidity, temperature);
    #endif
    #ifdef OLED_SCREEN
    oled::draw(humidity, temperature);
    #endif
    #ifdef SERVO_AS_POINTER
    servo.write(ramp_value(temperature, 10, 35, 150, 0));
    #endif
    
    delay(1000);
}
