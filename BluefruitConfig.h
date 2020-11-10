#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "Adafruit_BLEGatt.h"

#ifndef CONFIG_
#define CONFIG_                      
// COMMON SETTINGS
// ----------------------------------------------------------------------------------------------
// These settings are used in both SW UART, HW UART and SPI mode
// ----------------------------------------------------------------------------------------------
#define BUFSIZE                        160   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   true  // If set to 'true' enables debug output


// SOFTWARE UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following macros declare the pins that will be used for 'SW' serial.
// You should use this option if you are connecting the UART Friend to an UNO
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_SWUART_RXD_PIN       9    // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       10   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         11   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         -1   // Optional, set to -1 if unused

// SHARED UART SETTINGS
// ----------------------------------------------------------------------------------------------
// The following sets the optional Mode pin, its recommended but not required
// ----------------------------------------------------------------------------------------------
#define BLUEFRUIT_UART_MODE_PIN        12    // Set to -1 if unused
#define LED_PIN 7 
#define CONNECTED 1
#define NOT_CONNECTED  0
#define MAX_LIST_SIZE 18
#define REDO_CMD 1

typedef enum{
  bt_cmd_null,          // 0
  bt_gatt_clear,        // 1
  bt_led_service,       // 2
  bt_led_char,          // 3
  bt_set_connected,     // 4
  bt_set_disconnected,  // 5
  bt_set_device_name,   // 6
  bt_reset,             // 7
  bt_not_connectable,   // 8
  bt_connectable,       // 9
  bt_list_services,     // 10
  bt_no_echo,           // 11
  bt_info,              // 12
  bt_begin,             // 13
  bt_factory_reset,     // 14
  bt_connection_status, // 15
  bt_read_led_char,     // 16
} bt_cmd_t;     

const char NOT_CONNECTABLE[] = {"AT+GAPCONNECTABLE=0"};
const char CONNECTABLE[] = {"AT+GAPCONNECTABLE=1"};
const char SET_DEVICE_NAME[] = {"AT+GAPDEVNAME=climb-time"};
const char SET_POWER_LEVEL[] = {"AT+BLEPOWERLEVEL=4"};
const char GATT_CLEAR[] = {"AT+GATTCLEAR"};
const char LED_CHAR[] = {"AT+GATTADDCHAR=UUID=0x0001,PROPERTIES=8,MIN_LEN=1,MAX_LEN=23,DATATYPE=1,VALUE=XXXXXXXXXXXXXXXXXXXX,DESCRIPTION=LED"};
const char LIST_SERVICES[] = {"AT+GATTLIST"};
const char CONNECTION_STATUS[] = {"AT+GAPGETCONN"};
const char DISCONNECT[]={"AT+GAPDISCONNECT"};
const uint8_t SNAKE_TRANSLATE[] = {16, 17, 18, 19, 15, 14, 13, 12, 8, 9, 10, 11, 7, 6, 5, 4, 0, 1, 2 , 3};

void error(const __FlashStringHelper*err);                      
void Bluetooth_Initialize();
void Bluetooth_Send_Command(const char *command);
void Bluetooth_Create_Service();
void device_connected(void);
void device_disconnected(void);
void Check_LED_Value();
void getUserInput(char buffer[], uint8_t maxSize);
void BT_Poll();
void BT_Add_Command(bt_cmd_t command);
void BT_Pop_Command_Queue();
void BT_Send_Command(bt_cmd_t current_command);
uint8_t BT_Parse_Reply(bt_cmd_t command);
#endif // CONFIG_
