#include <Statistic.h>
#include <Timer.h>
// See https://tutoarduino.com/portfolio-items/8555/

#define OLED_SCREEN   1

#define SENSOR_PIN A0
#define BUZZER_PIN       3  // for smoothing val read from potentiometer
#define buzz(v) analogWrite(BUZZER_PIN, v)
#define BUTTON_PIN       8

#define SUBS_VALS        10
#define THRESH           300

#define SEC  *1000

#ifdef OLED_SCREEN
#include <U8glib.h>
namespace oled {
  U8GLIB_SSD1306_128X64 screen(U8G_I2C_OPT_NO_ACK);
  
  void setup() {
    screen.setColorIndex(1); // pixel on. My screen mode is U8G_MODE_BW    
    screen.setFont(u8g_font_gdr20r);
  }
  
  void draw(String x) {
    screen.firstPage();
    do {
      screen.setPrintPos(0, 24);
      screen.println(x);
    } while (screen.nextPage());
  }
}
#endif

float ramp_value(float x, float min_x, float max_x, float min_y, float max_y) {
  if (x > max_x)
    x = max_x;
  if (x < min_x)
    x = min_x;
  return min_y + (x - min_x)*(max_y - min_y)/(max_x - min_x);
}

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  #ifdef OLED_SCREEN
  oled::setup();
  #endif
}

void logTime(unsigned long x, bool braces = false) {
  int min = x/60, sec = x%60;
  String out = String("") + min + ":" + sec;
  Serial.println(out);

  #ifdef OLED_SCREEN
  oled::draw(String(braces ? "> " : "") + out + String(braces ? " <" : ""));
  #endif
}

Timer timer;
int countdown_action = -1;
//int ring_action = -1;
unsigned long final_millis;
void stop_buzz() {
  buzz(0);
}
void ring() {
  buzz(400);
  timer.after(0.6 SEC, stop_buzz);
}
void countdown() {
  unsigned long ct = (final_millis - millis()) / 1000;
  logTime(ct, true);
  //buzz(20);
  //timer.after(500, stop_buzz);
  Serial.println("Countdown...");
  if (ct <= 0) {
    ring();
    timer.every(1 SEC, ring, 3);
    countdown_action = -1;
  }
}

Statistic pot_vals;
bool button_was_high = false;
void loop() {
  pot_vals.add(analogRead(SENSOR_PIN));
  if(pot_vals.count() >= SUBS_VALS)
  {
    unsigned long x = (unsigned long)ramp_value(pot_vals.average(),0,1023,0,60*10);
    pot_vals.clear();
    bool button_is_high = digitalRead(BUTTON_PIN) == HIGH;
    bool button_rising_edge = !button_was_high && button_is_high;
    button_was_high = button_is_high;
    
    if (countdown_action == -1)
      logTime(x);
    if (button_rising_edge && countdown_action == -1) {
      logTime(x, true);
      final_millis = millis() + x*1000;
      countdown_action = timer.every(1 SEC, countdown, x);
    }
    else if (button_rising_edge) {
      timer.stop(countdown_action);
      countdown_action = -1;
      //timer.stop(ring_action);
      //ring_action = -1;
    }
  }
  //  delay(10);
  timer.update();
}
