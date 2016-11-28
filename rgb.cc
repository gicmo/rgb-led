#if ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

int led = 6;
int brightness = 0;
int fadeAmount = 5;

struct LED {
    LED(uint8_t pin, int br) : pin(pin), brightness(br) {}

    void setup() {
        pinMode(pin, OUTPUT);
    }

    void write() {
        analogWrite(pin, brightness);
    }

    uint8_t pin;
    int brightness;


};

struct Pulsor {
    Pulsor(const LED &l, int step=5, int max_br = 255, int min_br = 0)
            : led(l), min_br(min_br), max_br(max_br), step(step) { }

    void pulse() {
        int &brightness = led.brightness;

        analogWrite(led.pin, led.brightness);

        brightness += step;
        if (brightness >= max_br) {
            brightness = max_br;
            step *= -1;
        } else if (brightness <= min_br) {
            brightness = min_br;
            step *= -1;
        }
    }

    int min_br;
    int max_br;
    int step;

    LED led;
};

void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i;
  float f,p,q,t;

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

LED blue(6, 0);
LED green(3, 125);
LED red(5, 250);

float hue = 0.0;
int sat = 80;
bool up = false;

void setup() {
    blue.setup();
    green.setup();
    red.setup();
}


void loop() {

  hue += 3.6;
  if (hue > 360.0) {
    hue = 0.0;
  }

  if (sat < 30) {
    up = true;
  } else if (sat > 80) {
    up = false;
  }

  if (up) {
    sat += 1;
  } else {
    sat -= 1;
  }

  byte r, g, b;
  HSV_to_RGB(hue, sat, 50, &r, &g, &b);

  red.brightness = (byte) r * 0.75;
  green.brightness = g;
  blue.brightness = (byte) b * 0.4;

  red.write();
  green.write();
  blue.write();

  //wait for it
  delay(50);
}
