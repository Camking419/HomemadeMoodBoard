#include "LED_Strip.h"

LED_Colors hold_light_color;

Adafruit_NeoPixel led_strip(LED_COUNT, LED_PIN, NEO_GRB /*+ NEO_KHZ800*/);

uint32_t red;
uint32_t blue;
uint32_t green;
uint32_t white;
  
void LED_Strip_Initialize(){
  hold_light_color = color_white;

  led_strip.begin();
  led_strip.show();
  led_strip.setBrightness(LED_BRIGHTNESS);
  
  green = led_strip.Color(0, 64, 0);
  red = led_strip.Color(64, 0, 0);
  blue = led_strip.Color(0, 0, 64);
  white = led_strip.Color(21, 21, 21);

    for(uint8_t i = 0; i < 20; i++ ){
    led_strip.setPixelColor(i ,led_strip.Color(30, 30, 30));
    led_strip.show();
    delay(30);
    
  }
}

void Update_LED_Strip(const uint8_t bt_color, uint8_t index){
  uint32_t current_color;
  switch((LED_Colors)bt_color){
    case color_off:
      current_color = led_strip.Color(0, 0, 0);  
    break;
    case color_white:
      //Serial.println("White");
      current_color = white;
    break;
    case color_green:
     // Serial.println("Green");
      current_color = green;
    break;
    case color_red:
      //Serial.println("Red");
      current_color = red;
    break;
    case color_blue:
      //Serial.println("blue");
      current_color = blue;
    break;
  }
  
  led_strip.setPixelColor(index ,current_color);
  led_strip.show();
  delay(5);
}
