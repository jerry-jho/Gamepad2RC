/*
 Example sketch for the Xbox Wireless Reciver library - developed by Kristian Lauszus
 It supports up to four controllers wirelessly
 For more information see the blog post: http://blog.tkjelectronics.dk/2012/12/xbox-360-receiver-added-to-the-usb-host-library/ or
 send me an e-mail:  kristianl@tkjelectronics.com
 */

#include <XBOXRECV.h>
#include <SoftwareSerial.h>

#define MYPORT_RX 12
#define MYPORT_TX 14

SoftwareSerial myPort;

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
XBOXRECV Xbox(&Usb);
int cntr;
void setup() {
  pinMode(16,OUTPUT);
  digitalWrite(16, 1);
  Serial.begin(115200);

  myPort.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
  if (!myPort) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareSerial pin configuration, check config"); 
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }
  else {
    Serial.print(F("Serial OK"));
  }
  myPort.write(0xC0);

  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
  cntr = 0;
}
  uint8_t G1 = 0;
  uint8_t G1_get = 0;


void loop() {

  Usb.Task();
  if (Xbox.XboxReceiverConnected) {
    //Serial.print("OK");
    //sleep(1000);
    for (uint8_t i = 0; i < 4; i++) {
      if (Xbox.Xbox360Connected[i]) {
        cntr = cntr + 1;
        if (cntr < 500) {
            continue;
        }
        cntr = 0;
        G1_get = Xbox.getRAW(2);
        if (G1 != G1_get) {
            Serial.println(G1_get);
        }
        G1 = G1_get;
        uint8_t forward =  Xbox.getRAW(4);
        //Serial.println(lt);
        uint8_t backward =  Xbox.getRAW(5);
        //Serial.println(rt);
        uint8_t cc;
        if (forward == backward) {
            cc = 0xC0;
        } else if (forward > backward) {
            forward = forward - backward;
            forward = forward >> 4;
            cc = (0xC0 | forward);
        } else if (backward > forward) {
            backward = backward - forward;
            backward = backward >> 4;
            cc = (0xE0 | backward);
        }
        myPort.write(cc);
        
        //Serial.println(cc);
        uint8_t dir =  Xbox.getRAW(7);
         Serial.println(dir, HEX);
        if (dir != 0) {
            if (dir >= 128) {
                cc = 0xA2;
            } else {
                cc = 0x82;
            }
            
        }
        if (dir >= 128) { //left
           cc = 0xA0;
           dir = (256-dir)>>5;
           cc = cc | dir;
        } else {
           cc = 0x80;
           dir = (dir + 1)>>5;
           cc = cc | dir;
        }
        myPort.write(cc);
        Serial.println(cc, HEX);
      }
    }
  }
}
