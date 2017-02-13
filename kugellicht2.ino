#include "TimerOne.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

//pin for the data connection to the LED strip
#define PIN            6

// How many LEDs are attached to the Arduino?
#define NUMPIXELS      144

//max number of lights
#define NUM_KUGEL 5

//how many ticks for a new light
#define INTERVAL 600

//how many bits the weg value is shifted down to get a pleasant movement of the lights
#define  WEG_FAK 12

//const long  MAX_WAY=NUM_SEG<<WEG_FAK;
const long MAX_WAY=589824; // some trouble with long arithmetic

//array values are generated in python for a circle: 
//import math
//for i in range(144):
//    print(int(-math.cos(i/144*2*math.pi)*127),end=",")

char hoeheDiff[]= {-127,-126,-126,-125,-125,-123,-122,-121,-119,-117,-115,-112,-109,-107,-104,-100,-97,-93,-89,-85,-81,-77,-72,-68,-63,-58,-53,-48,-43,-38,-32,-27,-22,-16,-11,-5,0,5,11,16,22,27,32,38,43,48,53,58,63,68,72,77,81,85,89,93,97,100,104,107,109,112,115,117,119,121,122,123,125,125,126,126,127,126,126,125,125,123,122,121,119,117,115,112,109,107,104,100,97,93,89,85,81,77,72,68,63,58,53,48,43,38,32,27,22,16,11,5,0,-5,-11,-16,-22,-27,-32,-38,-43,-48,-53,-58,-63,-68,-72,-77,-81,-85,-89,-93,-97,-100,-104,-107,-109,-112,-115,-117,-119,-121,-122,-123,-125,-125,-126,-126};
long tempo[NUM_KUGEL],weg[NUM_KUGEL];

long ticks=0;
long next_tick=0;
byte akt_kugel=0;

byte r[NUM_KUGEL],g[NUM_KUGEL],b[NUM_KUGEL];
byte num_k_used;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

union  {
  uint32_t color;
  struct {
    byte r,g,b,a;
  } rgba;
} farbe;


void setup()
{
    pixels.begin();
  //pixels.setBrightness(100);
  for (byte i=0;i<NUMPIXELS;i++) { pixels.setPixelColor(i, 0);}
 pixels.show();
   
  //Serial.begin(115200);
  Timer1.initialize(20000);        
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

}
 
void callback()
{
  //a new light is added each INTERVAL ticks, replacing the oldest light
  if (ticks>=next_tick) {
    weg[akt_kugel]=0;
    tempo[akt_kugel]=random(10000);
    r[akt_kugel]=random(100);
    g[akt_kugel]=random(100);
    b[akt_kugel]=random(100);
    akt_kugel++;

    if (akt_kugel>num_k_used) {num_k_used=akt_kugel;}
    akt_kugel=(akt_kugel==NUM_KUGEL)?0:akt_kugel;
    next_tick+=INTERVAL;
     // Serial.println(ticks);
     //  Serial.println(akt_kugel);
     //   Serial.println(num_k_used);
  }

  //delete old lights
  for (byte i=0;i<NUMPIXELS;i++) { pixels.setPixelColor(i, 0);}

  //cycle through the used lights
  for (byte i=0;i<num_k_used;i++) {
    
  //the high resolution weg-var is shifted down to the number of LEDs
  byte seg=weg[i]>>WEG_FAK;
  
  byte strip_seg=NUMPIXELS-1-seg;

 //addition of the light colors to preexisting values
  farbe.color = pixels.getPixelColor(strip_seg);
  farbe.rgba.r+=r[i];
  farbe.rgba.g+=g[i];
  farbe.rgba.b+=b[i];
  pixels.setPixelColor(strip_seg, farbe.color);
 

  //the diff in height between the LEDs is equivalent to the negative force, so the height diff can be subsracted from the velocity (a way up is deaccelerating, a way down accelerating)  
  tempo[i]-=hoeheDiff[seg];

  //a small amount of the velocity is substracted for friction 
  tempo[i]=tempo[i]-(tempo[i]>>10);

  //each tick the light is moving according to the velocity
  weg[i]=weg[i]+tempo[i];

  //to close the circle a light who is leaving the LED-strip in each direction is remapped
  if (weg[i]>MAX_WAY) weg[i]-=MAX_WAY;
  if (weg[i]<0) weg[i]+=MAX_WAY;
  }
  
  //display the lights
  pixels.show();
  
  /*
  Serial.print("Seg:");
  Serial.print(seg);
  Serial.print(" Tempo:");
  Serial.print(tempo);
  Serial.print(" Weg:");
  Serial.println(weg>>WEG_FAK);
  */
  
  ticks++;
}
 
void loop()
{
  // nothing to do in the main loop, everything is done in the timer interrupt callback routine
}
