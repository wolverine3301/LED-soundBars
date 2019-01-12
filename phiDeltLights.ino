//Written by Logan Collier

#include <Adafruit_NeoPixel.h>  //neopixel library
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6 // DIN Pin for neoPixels
#define NUMPIXELS 75 //Number of pixels in stick

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second
int level = 0;
int h = 0;
int colors[] = {0, 0, 0}; // colors value for Red, Blue, Green

//these values represent the middle of each bar, change these values to achieve the middle of yours
//if you are using less bars feel free to delete the code dealing with the extra variables
//if you are using more bars you will have to make more of these variables and duplicate the code 
//with the previous variables and replace it with the new ones
int x = 11;
int y = 36;
int z = 58;
int a = 99;

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  Serial.begin(9600);
  pixels.begin(); // This initializes the NeoPixel library.
  for (uint16_t i = 0; i < pixels.numPixels(); i++) {
    pixels.setPixelColor(i, 0);        //turn pixel off
    delay(1);
  }

}

void loop() {
  level = 0;
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.

  // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(A0);
    if (sample < 1024) { // toss out spurious readings

      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  Serial.println(h);

  peakToPeak = signalMax - signalMin;
  //VU meter visuilization
  colors[h] = 100; //RED starting

  //Mic reading range for each level
  if (peakToPeak > 550) {
    level = 12;
    //set all values to 0
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
    //h is a counter. if h = 0 color = red. if h = 1 color = green. if h = 2 color = purple.
    if (h == 3) {
      h = 0;
      colors[h] = 100;
    }
    else {
      colors[h] = 100;
      h++;
    }
  }
  else if (peakToPeak > 525 && peakToPeak < 550) {
    level = 11;
    //set all values to 0
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
    //h is a counter. if h = 0 color = red. if h = 1 color = green. if h = 2 color = purple.
    if (h + 1 == 3) {
      h = 0;
      colors[h] = 100;
    }
    else {
      colors[h] = 100;
      h++;
    }
  }
  else if (peakToPeak > 500 && peakToPeak < 525) {
    level = 10;
    //set all values to 0
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
    //h is a counter. if h = 0 color = red. if h = 1 color = green. if h = 2 color = purple.
    if (h + 1 == 3) {
      h = 0;
      colors[h] = 100;
    }
    else {
      colors[h] = 100;
      h++;
    }
  }
  //When the VU meter hits The max Value The color will Change
  else if (peakToPeak > 475 && peakToPeak < 500) {
    level = 9;
    //set all values to 0
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;
    //h is a counter. if h = 0 color = red. if h = 1 color = green. if h = 2 color = purple.
    if ((h + 1) == 3) {
      h = 0;
      colors[h] = 100;
    }
    else {
      colors[h] = 100;
      h++;
    }

  }
  else if (peakToPeak > 450 && peakToPeak < 475) level = 8;
  else if (peakToPeak > 400 && peakToPeak < 450) level = 7;
  else if (peakToPeak > 350 && peakToPeak < 400) level = 6;
  else if (peakToPeak > 250 && peakToPeak < 300) level = 5;
  else if (peakToPeak > 200 && peakToPeak < 250) level = 4;
  else if (peakToPeak > 150 && peakToPeak < 200) level = 3;
  else if (peakToPeak > 100 && peakToPeak < 150) level = 2;
  else {
    level = 0;
  }

  spectra(level);

}
//sets each pixel based on number. X Y Z and A are values that represent a pixel in the middle of each strip
// the volume level lights up LEDs outwords from this middle value. by adding more of these values you can change how many of these effects happen per strip
// as of now there is 1 per 4 strips
void spectra(int level) {
  for (uint16_t i = 0; i < level; i++) {
    //first
    pixels.setPixelColor(x + i, pixels.Color(colors[0], colors[1], colors[2]));
    pixels.setPixelColor(x - i, pixels.Color(colors[0], colors[1], colors[2]));
    //second
    pixels.setPixelColor(y + i, pixels.Color(colors[0], colors[1], colors[2]));
    pixels.setPixelColor(y - i, pixels.Color(colors[0], colors[1], colors[2]));
    //third
    pixels.setPixelColor(z + i, pixels.Color(colors[0], colors[1], colors[2]));
    pixels.setPixelColor(z - i, pixels.Color(colors[0], colors[1], colors[2]));
    //fourth
    pixels.setPixelColor(a + i, pixels.Color(colors[0], colors[1], colors[2]));
    pixels.setPixelColor(a - i, pixels.Color(colors[0], colors[1], colors[2]));
    pixels.show();
    delay(1);
  }

  for (uint16_t i = level; i > 0; i--) {
    for (int c = 0; c < 100; c += 5) {
      //first
      pixels.setPixelColor(x + i, pixels.Color(colors[0], colors[1], colors[2]));
      pixels.setPixelColor(x - i, pixels.Color(colors[0], colors[1], colors[2]));
      //second
      pixels.setPixelColor(y + i, pixels.Color(colors[0], colors[1], colors[2]));
      pixels.setPixelColor(y - i, pixels.Color(colors[0], colors[1], colors[2]));
      //third
      pixels.setPixelColor(z + i, pixels.Color(colors[0], colors[1], colors[2]));
      pixels.setPixelColor(z - i, pixels.Color(colors[0], colors[1], colors[2]));
      //fourth
      pixels.setPixelColor(a + i, pixels.Color(colors[0], colors[1], colors[2]));
      pixels.setPixelColor(a - i, pixels.Color(colors[0], colors[1], colors[2]));
      pixels.show();
      colors[h]--;

    }
    colors[0] = 0;
    colors[1] = 0;
    colors[2] = 0;

    //first
    pixels.setPixelColor(x + i, 0);
    pixels.setPixelColor(x - i, 0);
    //second
    pixels.setPixelColor(y + i, 0);
    pixels.setPixelColor(y - i, 0);
    //third
    pixels.setPixelColor(z + i, 0);
    pixels.setPixelColor(z - i, 0);
    //fourth
    pixels.setPixelColor(a + i, 0);
    pixels.setPixelColor(a - i, 0);
    pixels.show();
    delay(1);
  }

}
