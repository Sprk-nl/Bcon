/*
port A101KWT2
*/

 #include <ELECHOUSE_CC1101.h>
 
 // constants won't change. Used here to 
// set pin numbers:
const int ledPin =  8;      // the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // interval at which to blink (milliseconds)

// cc1101 receive buffer
byte RX_buffer [61] = {0}; 
byte size, i, flag;

byte protocol_version [2] = {0};
byte protocol_address_src [8] = {0};
byte protocol_address_dst [8] = {0};
byte protocol_interface [2] = {0};
byte protocol_function [4] = {0};
byte protocol_data [32] = {0};
byte protocol_crc [2] = {0};

int msb_protocol_version = 0;
int lsb_protocol_version = 1;
int msb_protocol_address_src = 2;
int lsb_protocol_address_src = 9;
int msb_protocol_address_dst = 10;
int lsb_protocol_address_dst = 17;
int msb_protocol_interface = 18;
int lsb_protocol_interface = 19;
int msb_protocol_function = 20;
int lsb_protocol_function = 23;
int msb_protocol_data = 24;
int lsb_protocol_data = 25;
int msb_protocol_crc = 56;
int lsb_protocol_crc = 57;

 void setup()
{
  Serial.begin(9600);
  Serial.println("Hello World - Node A");
  Serial.println("Waiting for Node-B data to send to serial");
  Serial.println("receiving in interrupt-mode");
  radio_init();
  radio_get_configreg();
  pinMode(ledPin, OUTPUT);
  attachInterrupt (0, ELECHOUSE_CC1101_RevData,FALLING);
  
  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);
  pinMode(A1, OUTPUT);
  digitalWrite(A1, LOW);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);
  pinMode(A5, OUTPUT);
  digitalWrite(A5, LOW);

}



void loop()
{
blinker();
delay (100);
}


void ELECHOUSE_CC1101_RevData() {
// this function is run via a interrupt
decode_buffer();
// decode_data();
show_buffer();
ELECHOUSE_cc1101.SetReceive(); 
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
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    }
    else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void  decode_buffer() {
  size=ELECHOUSE_cc1101.ReceiveData(RX_buffer);
  for(i=0;i<size;i++)
  {
    Serial.print (RX_buffer [i], DEC);

     if ( (i >=  msb_protocol_version) && ( i < (msb_protocol_version+sizeof(protocol_version) ) ) ) {
      protocol_version[i-msb_protocol_version] = RX_buffer[i];
     }
      
     if ( (i >=  msb_protocol_address_src) && ( i < (msb_protocol_address_src+sizeof(protocol_address_src) ) ) ) {
      protocol_address_src[i-msb_protocol_address_src] = RX_buffer[i];
    }
    
     if ( (i >=  msb_protocol_address_dst) && ( i < (msb_protocol_address_dst+sizeof(protocol_address_dst) ) ) ) {
      protocol_address_dst[i-msb_protocol_address_dst] = RX_buffer[i];
    }
    
     if ( (i >=  msb_protocol_interface) && ( i < (msb_protocol_interface+sizeof(protocol_interface) ) ) ) {
      protocol_interface[i-msb_protocol_interface] = RX_buffer[i];
    }

     if ( (i >=  msb_protocol_function) && ( i < (msb_protocol_function+sizeof(protocol_function) ) ) ) {
      protocol_function[i-msb_protocol_function] = RX_buffer[i];
    }

     if ( (i >=  msb_protocol_data) && ( i < (msb_protocol_data+sizeof(protocol_data) ) ) ) {
      protocol_data[i-msb_protocol_data] = RX_buffer[i];
    }

     if ( (i >=  msb_protocol_crc) && ( i < (msb_protocol_crc+sizeof(protocol_crc) ) ) ) {
      protocol_crc[i-msb_protocol_crc] = RX_buffer[i];
    }
    
  }
    Serial.println ("");
}






void show_buffer() {
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
}


