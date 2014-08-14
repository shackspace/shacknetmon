#include <Adafruit_NeoPixel.h>
#include <SimpleTimer.h>

SimpleTimer timer; 
#define STRIPLENGTH 114
#define PIN 15

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPLENGTH, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  strip.begin();
  blackout();
  Serial.println("Init");
  strip.show();
  timer.setInterval(100, fadePercentage);
}
int target_perc = 0;
int real_perc = 0;
void loop() {
  if (Serial.available() > 0) {
    int perc = Serial.parseInt();
    if (Serial.read() == '\n') {
      setNewPercentage(perc);
    }
  }
  timer.run();
}

void setNewPercentage(uint8_t percentage) {
  target_perc = percentage;    
}
void fadePercentage() {
  if (target_perc > real_perc) {
    Serial.print("Target: ");
    Serial.print(target_perc);
    Serial.print(" Old: ");
    Serial.print(real_perc);
    real_perc++;
    setPercentage(real_perc);
    Serial.print(" New: ");
    Serial.println(real_perc);
  }else if (target_perc < real_perc) {
          Serial.print("Target: ");
    Serial.print(target_perc);
    Serial.print(" Old: ");
    Serial.print(real_perc);

    real_perc--;
    setPercentage(real_perc);
    Serial.print(" New: ");
    Serial.println(real_perc);
  }else { 
    return;
  }

}
void setPercentage(uint8_t percentage) {
  blackout();
  float p = percentage / 100.0;
  uint8_t steps = STRIPLENGTH * p;
  float hstepsize = 150 / STRIPLENGTH;
  for(uint32_t i = 0; i < steps; i++) {
    byte r,g,b;
    HSV_to_RGB(160 - (i * hstepsize), 100, 100, &r, &g, &b);
    setColor(i, r, g, b);
  }
  strip.show();
}  
  

void setColor(uint32_t led, byte r, byte g, byte b) {
  uint32_t c = strip.Color(r,g,b);
  strip.setPixelColor(led, c);
}

void blackout() {
  uint32_t i;
  uint32_t c = 0x000000;
  for(i = 0; i < STRIPLENGTH; i++) {
    strip.setPixelColor(i, c);
  }
}

void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i,f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }
 
  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}
