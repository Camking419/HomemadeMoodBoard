#include "BluefruitConfig.h"
#include "Adafruit_BLE.h"
#include "LED_Strip.h"

uint8_t done_initializing = 0;
uint8_t connection_status = CONNECTED;
uint8_t char_buffer[20];
uint8_t char_buffer_length = 20;
uint8_t num_commands = 0;
int32_t reply;
uint8_t LED_SERVICE[] = {0x73, 0x47, 0x32, 0xcb, 0xc2, 0x48, 0x4b, 0x47, 0xa7, 0x9f, 0x22, 0x68, 0xc7, 0x8b, 0x0a, 0xbc};
bt_cmd_t bt_commands[MAX_LIST_SIZE]; 


SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);


Adafruit_BLEGatt bleGatt(ble);
                      
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void Bluetooth_Initialize(){
  Serial.println(F("Adafruit Bluefruit Command Mode"));
  Serial.println(F("---------------------------------------"));

  /* LED Pin */
  pinMode(LED_PIN, OUTPUT);
   
  /* Initialise the module */

  
  Serial.print(F("Initialising the Bluefruit LE module: "));
  BT_Add_Command(bt_begin);
  //BT_Add_Command(bt_factory_reset);
  BT_Add_Command(bt_no_echo);
  BT_Add_Command(bt_info);
  BT_Add_Command(bt_gatt_clear);
  BT_Add_Command(bt_not_connectable);
  BT_Add_Command(bt_led_service);
  BT_Add_Command(bt_led_char);
  BT_Add_Command(bt_set_device_name);

  BT_Add_Command(bt_set_connected);
  BT_Add_Command(bt_set_disconnected);
  BT_Add_Command(bt_connectable);
  BT_Add_Command(bt_reset);
  BT_Add_Command(bt_list_services);
 
}



void BT_Poll(){
  if(num_commands){
    BT_Send_Command(bt_commands[0]); 
    return;
  } 
  if(done_initializing){
      BT_Add_Command(bt_connection_status);
      Check_LED_Value();
      char command[BUFSIZE+1];
      getUserInput(command, BUFSIZE);
      
 
  }
  
}

void BT_Add_Command(bt_cmd_t command){
  bt_commands[num_commands] = command;
  num_commands++;
}

void BT_Pop_Command_Queue(){
   uint8_t i;
   bt_cmd_t temp;
   
  if(num_commands == MAX_LIST_SIZE){
    BT_Send_Command(bt_commands[0]);
    return;
  }
 
  for(i = 0; i < num_commands-1; i++){
    temp = bt_commands[i+1];
    bt_commands[i] = temp;
  }
  num_commands--;
  
}

void BT_Send_Command(bt_cmd_t current_command){
   
   switch(current_command){
      case bt_cmd_null:
        Serial.println("Command Q Error");
        break;
      case bt_gatt_clear:
        ble.println(GATT_CLEAR);
        break;
      case bt_led_service:
        delay(200);
        bleGatt.addService(LED_SERVICE);
        break;
      case bt_led_char:
        delay(200);
        ble.println(LED_CHAR);
        break;
      case bt_set_connected:
        delay(100);
        ble.setConnectCallback(device_connected);
        break;    
      case bt_set_disconnected:
        ble.setDisconnectCallback(device_disconnected);
        break; 
      case bt_set_device_name:
        delay(200);
        ble.println(SET_DEVICE_NAME);
        break;
      case bt_reset:
        ble.reset();
        break;
      case bt_not_connectable:
        ble.println(NOT_CONNECTABLE);
        break;
      case bt_connectable:
        ble.println(CONNECTABLE);
        break;    
      case bt_list_services:
        ble.println(LIST_SERVICES);
        done_initializing = 1;
        break;               
      case bt_no_echo:
        ble.echo(false);
        break;
      case bt_info:
        ble.info();
        break;
      case bt_begin:
        if ( !ble.begin(VERBOSE_MODE) )
        {
          Serial.println("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
        }
        break;
      case bt_factory_reset:
        if ( ! ble.factoryReset() ){
          Serial.println("Couldn't factory reset");
        }  
        break;  
      case bt_connection_status:
        ble.println(CONNECTION_STATUS);
        break;
      case bt_read_led_char:
        bleGatt.getChar(1, char_buffer,char_buffer_length );
        Serial.println("New char!!!") ;
        for(int i=0; i< char_buffer_length; i++){
          Update_LED_Strip(char_buffer[i], SNAKE_TRANSLATE[i] );
          Serial.print(char_buffer[i]);
        }
        Serial.println();  
        break;

      
   }
   
   if(BT_Parse_Reply(current_command)){
      Serial.print("Buf: ");
      Serial.println(ble.buffer);
      BT_Send_Command(current_command);
   }
        
   BT_Pop_Command_Queue();
   
}

uint8_t BT_Parse_Reply(bt_cmd_t command){
  uint8_t rx_buf[15];
  uint16_t buf_size = 15;
  uint16_t timeout = 10;
  uint8_t i;
  
  switch(command){
      case bt_cmd_null:
        return 0;
      case bt_set_connected:    
      case bt_set_disconnected:      
      case bt_led_service:
          ble.waitForOK();
          Serial.print("Buf: ");
          Serial.println(ble.buffer);
          if((ble.buffer[0] == '1')|| ((ble.buffer[0] == 'O') && (ble.buffer[1] == 'K'))){
            return 0;
          }
          else{
            return 1;
          }
      case bt_led_char:    
      case bt_gatt_clear:  
      case bt_set_device_name:
      case bt_not_connectable:       
       return !(ble.waitForOK());
      case bt_factory_reset:
      case bt_connectable:
          if(ble.waitForOK()){
            return 0; 
          }
          Serial.print("Buf: ");
          Serial.println(ble.buffer[0], DEC);
          if( (ble.buffer[0] == -1) || ((ble.buffer[0] == 'O') && (ble.buffer[1] == 'K')) ){
            return 0;
          }
          else{
            return 1;
          } 
      case bt_connection_status:
          ble.waitForOK();
          Serial.print("Buf: ");
          Serial.println(ble.buffer);           
      case bt_read_led_char:
      case bt_reset:  
      case bt_no_echo:              
      case bt_info:
      case bt_begin:
        return 0; 
      case bt_list_services:
        Serial.println("Done Initializing");
        done_initializing = 1;
        return 0;           
  }
  
}
void device_connected(void){
  connection_status = CONNECTED;
  Serial.println("Connected");
}

void device_disconnected(void){
  connection_status = NOT_CONNECTED;
  Serial.println("Disconnected");
}

void Check_LED_Value(){
  if(!connection_status){
    return;
  }
  else{
    BT_Add_Command(bt_read_led_char);
/*
   for(int i = 0; i< char_buffer_length; i++){
      switch(char_buffer[i]){
        case 'X':
          char_buffer[i] = 'W';
        break; 
        case 'W':
          char_buffer[i] = 'R';
        break;
        case 'R':
          char_buffer[i] = 'G';
        break;
        case 'G':
          char_buffer[i] = 'B';
        break;
        case 'B':
          char_buffer[i] = 'X';
        break;
      }
   }
*/
  }
}

void getUserInput(char buffer[], uint8_t maxSize)
{
  memset(buffer, 0, maxSize);

  uint8_t count=0;

  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.available() == 0) );
}
