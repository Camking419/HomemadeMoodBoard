#include "LED_Strip.h"
#include "BluefruitConfig.h"

#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

#define FACTORYRESET_ENABLE      0
#define BUFSIZE                  160 

// Create the bluefruit object, either software serial...uncomment these lines

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    LED_Strip_Initialize();
    Bluetooth_Initialize();

    
    
}

void loop() {
  // put your main code here, to run repeatedly:
  BT_Poll();
  delay(50);
}
