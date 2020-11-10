#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define LED_COUNT 30
#define LED_BRIGHTNESS 50




typedef enum{
  color_off = 'X',    // 0
  color_white = 'W',  // 1
  color_green = 'G',  // 2
  color_red = 'R',    // 3
  color_blue = 'B',   // 4
}LED_Colors;
void LED_Strip_Initialize();
void Update_LED_Strip(const uint8_t bt_color, uint8_t index);
