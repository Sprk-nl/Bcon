/* 
Board A9007NTD
*/

#include <ELECHOUSE_CC1101.h>
#define size 61

byte TX_buffer[size]={0};
byte i;

 // constants won't change. Used here to 
// set pin numbers:
const int ledPin =  8;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long LED_previousMillis = 0;        // will store last time LED was updated
long data_send_previousMillis = 0;        // will store last time data was send
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long LED_interval = 1000;           // interval at which to blink (milliseconds)
long data_send_interval = 1000;           // interval at which to blink (milliseconds)
int x = 0;

byte protocol_version [2] = {0};
byte protocol_address_src [8] = {0};
byte protocol_address_dst [8] = {0};
byte protocol_interface [2] = {0};
byte protocol_function [4] = {0};
byte protocol_data [32] = {0};
byte protocol_crc [2] = {0};

 void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World - Node B");
  radio_init();
  radio_get_configreg();
  pinMode(ledPin, OUTPUT); 

//  for(i=0;i<size;i++) {
//    TX_buffer [i] = i;
//    }

}

void loop()
{
data_send();
blinker();
delay (100);
}



int radio_get_configreg() {
      Serial.print("Channel number: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_CHANNR),DEC);
      Serial.print("Device address: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_ADDR),DEC);
      Serial.print("Packet automation control 1: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_PKTCTRL1),DEC);
      Serial.print("Packet automation control 0: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_PKTCTRL0),DEC);
      Serial.print("Packet length: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_PKTLEN),DEC);
      Serial.print("CC1101_DEVIATN: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_DEVIATN),DEC);
      Serial.print("CC1101_MDMCFG0: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG0),DEC);
      Serial.print("CC1101_MDMCFG1: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG1),DEC);
      Serial.print("CC1101_MDMCFG2: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG2),DEC);
      Serial.print("CC1101_MDMCFG3: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG3),DEC);
      Serial.print("CC1101_MDMCFG4: ");
      Serial.println(ELECHOUSE_cc1101.SpiReadReg(CC1101_MDMCFG4),DEC);
      Serial.println( " ");
}


void radio_init() {
  ELECHOUSE_cc1101.Init();
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_CHANNR, 5); // Change channel 
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_PKTCTRL0, 68); // fixed packet length mode & data whitening on
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_PKTCTRL1, 12); // Address check and 0 + 255, append status, crc_autoflush
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_PKTLEN, 61); // packet length
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_ADDR, 7); // address
  ELECHOUSE_cc1101.SpiWriteReg(CC1101_MDMCFG1, 128); // FEC
  ELECHOUSE_cc1101.SetReceive();
}

void blinker() {
  // here is where you'd put code that needs to be running all the time.

  // check to see if it's time to blink the LED; that is, if the 
  // difference between the current time and last time you blinked 
  // the LED is bigger than the interval at which you want to 
  // blink the LED.
  unsigned long LED_currentMillis = millis();
 
  if(LED_currentMillis - LED_previousMillis > LED_interval) {
    // save the last time you blinked the LED 
    LED_previousMillis = LED_currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}


void data_send() {
  unsigned long data_send_currentMillis = millis();
 
  if(data_send_currentMillis - data_send_previousMillis > data_send_interval) {
    // save the last time you blinked the LED 
    data_send_previousMillis = data_send_currentMillis;   
//    ELECHOUSE_cc1101.SendData(TX_buffer, size);


byte protocol_version [2] = {0xFF,0xFE};
byte protocol_address_src [8] = {0x11,0x01,0x01,0x01,0x01,0x01,0x01,0x12};
byte protocol_address_dst [8] = {0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x02};
byte protocol_interface [2] = {0x03,0x03};
byte protocol_function [4] = {0x04,0x04,0x04,0x04};
byte protocol_data [32] = {0x05,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x05};
byte protocol_crc [2] = {0x06,0x06};


for (int x=0; x < sizeof(protocol_version); x++) {
    TX_buffer [x] = protocol_version[x];
    }
for (int x=0; x < sizeof(protocol_address_src); x++) {
    TX_buffer [x+2] = protocol_address_src[x];
    }
for (int x=0; x < sizeof(protocol_address_dst); x++) {
    TX_buffer [x+10] = protocol_address_dst[x];
    }
for (int x=0; x < sizeof(protocol_interface); x++) {
    TX_buffer [x+18] = protocol_interface[x];
    }
for (int x=0; x < sizeof(protocol_function); x++) {
    TX_buffer [x+20] = protocol_function[x];
    }
for (int x=0; x < sizeof(protocol_data); x++) {
    TX_buffer [x+24] = protocol_data[x];
    }
for (int x=0; x < sizeof(protocol_crc); x++) {
    TX_buffer [x+56] = protocol_crc[x];
    }

Serial.println("Data to send:");

Serial.print ("protocol_version     :");
for (int x=0; x < sizeof(protocol_version); x++) {
    Serial.print (protocol_version [x], DEC); 
    Serial.print(" ");
    }
Serial.println();

Serial.print ("protocol_address_src :");
for (int x=0; x < sizeof(protocol_address_src); x++) {
    Serial.print (protocol_address_src [x], DEC); 
    Serial.print(" ");
    }
Serial.println();
Serial.print ("protocol_address_dst :");
for (int x=0; x < sizeof(protocol_address_dst); x++) {
    Serial.print (protocol_address_dst [x]); 
    Serial.print(" ");
    }
Serial.println();

Serial.print ("protocol_interface   :");
for (int x=0; x < sizeof(protocol_interface); x++) {
    Serial.print (protocol_interface [x]);
    Serial.print(" ");
  }
Serial.println();

Serial.print ("protocol_function    :");
for (int x=0; x < sizeof(protocol_function); x++) {
    Serial.print (protocol_function [x]);
    Serial.print(" ");
  }
Serial.println();

Serial.print ("protocol_data        :");
for (int x=0; x < sizeof(protocol_data); x++) {
    Serial.print (protocol_data [x]);
    Serial.print(" ");
  }
Serial.println();

Serial.print ("protocol_crc         :");
for (int x=0; x < sizeof(protocol_crc); x++) {
    Serial.print (protocol_crc [x]);
    Serial.print(" ");
  }
Serial.println();
Serial.println(); // extra whiteline to mark the end

for(i=0;i<size;i++)
{

  // Serial.print (TX_buffer [i], DEC); // Send the whole tx_buffer byte to serial.
 }
Serial.println();      
ELECHOUSE_cc1101.SendData(TX_buffer, size);
}

  }

