/*
  UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 
 A Processing sketch is included at the end of file that can be used to send 
 and received messages for testing with a computer.
 
 created 21 Aug 2010
 by Michael Margolis
 
 This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include <TimerOne.h>

#include "LPD8806.h"

// Number of RGB LEDs in strand:
const int nLEDs = 32;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

void transitionEffect();
void callback();

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

byte imgData[nLEDs * 3]; // Data for 1 long strips worth of imagery
int  fxVars[50];             // Effect instance variables (explained later)

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

//IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
//char  ReplyBuffer[] = "acknowledged";       // a string to send back
char buf[50];
int pixelPOS;
byte pixelR;
byte pixelG;
byte pixelB;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

char remoteIPAddy[50];
char addyPart[8];


void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac);
  Udp.begin(localPort);
  
  Serial.begin(9600);
  
  // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    sprintf(addyPart,"%d.",Ethernet.localIP()[thisByte]);
    sprintf(remoteIPAddy,"%s%s",remoteIPAddy,addyPart);
    
  memset(imgData, 0, sizeof(imgData)); // Clear image data
  fxVars[0] = 0;           // Mark back image as initialized
    
  // Timer1 is used so the strip will update at a known fixed frame rate.
  // Each effect rendering function varies in processing complexity, so
  // the timer allows smooth transitions between effects (otherwise the
  // effects and transitions would jump around in speed...not attractive).
  Timer1.initialize();
  Timer1.attachInterrupt(callback, 1000000 / 60); // 60 = 60 frames/second
    
  }
  Serial.print(remoteIPAddy); 
  
  pixelPOS = 0;
  byte pixelR = 90;
  byte pixelG = 90;
  byte pixelB = 90;

  // Start up the LED strip
  strip.begin();
  // Start by turning all pixels off:
  for(int i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);
  // Update the strip, to start they are all 'off'
  strip.show();
  
}

void loop() {
  
  // Do nothing.  All the work happens in the callback() function below,
  // but we still need loop() here to keep the compiler happy.
  // Still need to remove this code
  
  
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
 
    //Serial.print("From ");
    //IPAddress remote = Udp.remoteIP();
    /*
    for (int i =0; i < 4; i++)
    {
      //Serial.print(remote[i], DEC);
      if (i < 3)
      {
        //Serial.print(".");
      }
    }
    */
    //Serial.print(", port ");
    //Serial.println(Udp.remotePort());
    
    // CLear the packet buffer of last message
    memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    
    if(strcmp(packetBuffer,"Left Arrow") == 0 )
    {
      pixelPOS--;
      Serial.print("Left Arrow");
    }
    if(strcmp(packetBuffer,"Right Arrow") == 0 )
    {
      pixelPOS++;
      Serial.print("Right Arrow");
    }
    if(strcmp(packetBuffer,"Up Arrow") == 0 )
    {
      Serial.print("Up Arrow");
      brightnessUp();
    }
    if(strcmp(packetBuffer,"Down Arrow") == 0 )
    {
      Serial.print("Down Arrow");
      brightnessDown();
    }
    if(strcmp(packetBuffer,"R") == 0 )
    {
      Serial.print("R");
      changeR();
    }
    if(strcmp(packetBuffer,"G") == 0 )
    {
      Serial.print("G");
      changeG();
    }
    if(strcmp(packetBuffer,"B") == 0 )
    {
      Serial.print("B");
      changeB();
    }
    
    strip.setPixelColor(pixelPOS, strip.Color(pixelR, pixelG, pixelB)); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(pixelPOS, 0); // Erase pixel, but don't refresh!
    delay(50);
    
    
    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
  delay(10);
}

void brightnessUp(){
  if (pixelR<127) 
  {
    pixelR++;
  }
  if (pixelG<127) 
  {
     pixelG++;
  }
  if (pixelB<127) 
  {
     pixelB++;
  }
}

void brightnessDown(){
  if (pixelR > 0) 
  {
     pixelR--;
  }
  if (pixelG > 0) 
  {
     pixelG--;
  }
  if (pixelB > 0) 
  {
     pixelB--;
  }
}

void changeR() {
 if(pixelR<127) { 
    pixelR+=5; 
  } else {
    pixelR = 0;
  }
}

void changeG() {
 if(pixelG<127) { 
    pixelG+=5; 
  } else {
    pixelG = 0;
  }}

void changeB() {
 if(pixelB<127) { 
    pixelB+=5; 
  } else {
    pixelB = 0;
  }
}

// Timer1 interrupt handler.  Called at equal intervals; 60 Hz by default.
void callback() {
  // Very first thing here is to issue the strip data generated from the
  // *previous* callback.  It's done this way on purpose because show() is
  // roughly constant-time, so the refresh will always occur on a uniform
  // beat with respect to the Timer1 interrupt.  The various effects
  // rendering and compositing code is not constant-time, and that
  // unevenness would be apparent if show() were called at the end.
  strip.show();

  byte *backPtr    = &imgData[0],
       r, g, b;
  int  i;

  // Always render back image based on current effect index:
  (*transitionEffect)();

   // No transition in progress; just show back image
    for(i=0; i<nLEDs; i++) {
      // See note above re: r, g, b vars.
      r = *backPtr++;
      g = *backPtr++;
      b = *backPtr++;
      strip.setPixelColor(i, r, g, b);
    }
  }
