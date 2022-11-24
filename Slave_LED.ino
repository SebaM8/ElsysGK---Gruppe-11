#include <FastLED.h>
#include <Wire.h>

#define NUM_LEDS 20
#define NUM_STRIPS 8

int tid = 1000;
unsigned long currentMillis;
unsigned long startMillis;
unsigned long periode = 80;
int t = 0;
bool k = false;
uint8_t gBrightness = 255;
CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];

int g = 2 ;

void setup() { 
   
  startMillis = millis();
  controllers[0] = &FastLED.addLeds<WS2812,9>(leds, NUM_LEDS);
  controllers[1] = &FastLED.addLeds<WS2812,3>(leds, NUM_LEDS);
  controllers[2] = &FastLED.addLeds<WS2812,7>(leds, NUM_LEDS);
  controllers[5] = &FastLED.addLeds<WS2812,4>(leds, NUM_LEDS);
  controllers[6] = &FastLED.addLeds<WS2812,6>(leds, NUM_LEDS);
  controllers[3] = &FastLED.addLeds<WS2812,10>(leds, NUM_LEDS);
  controllers[7] = &FastLED.addLeds<WS2812,8>(leds, NUM_LEDS);
  controllers[8] = &FastLED.addLeds<WS2812,5>(leds, NUM_LEDS);
  
  

  Serial.begin(115200);
  Wire.begin(50);
  Wire.onReceive(receiveEvent);

}  

void receiveEvent(int bytes) {
  g = Wire.read();    
}

void loop() { 

Led_strip1(g);


}
void Led_strip1(int g)
{
  FastLED.clear();
  FastLED.show();

    
  while( t <= 19){
    k = false;
    startMillis = millis();
    
    while (k == false){
      if (t <=2){
            leds[t] = CRGB::White;
            if ((currentMillis - startMillis)>periode){
              controllers[g]->showLeds(gBrightness);
              k = true;
            }   
        } 
       else if (t <=13) {
        leds[t-3] = CRGB::Black;
        leds[t] = CRGB::White;
        if ((currentMillis - startMillis)>periode){
          controllers[g]->showLeds(gBrightness);
          k = true;   
        }
       }
        else{
          leds[t] = CRGB::White;
          if ((currentMillis - startMillis)>periode){
            leds[t] = CRGB::White;
            controllers[g]->showLeds(gBrightness);
            k=true;
          }
        }
        

       
        
        currentMillis = millis();
      }
      t += 1;
      Serial.print(t);
 }


  
for (int i = 11; i <= 19; i++) {
leds[i] = CRGB::Green;
}
controllers[g]->showLeds(gBrightness); 
t = 0;
Serial.println(g);
}
