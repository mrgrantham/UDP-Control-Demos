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
#include <UTFT.h>

// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Declare an instance of the class
UTFT myGLCD(PCF8833,6,5,2,3,4);   // Remember to change the model parameter to suit your display module!


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

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

char remoteIPAddy[50];
char addyPart[8];
void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac);
  Udp.begin(localPort);

  // Setup the LCD
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.setFont(SmallFont);

  //Serial.begin(9600);
  
  myGLCD.clrScr();
  // print your local IP address:
  //Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    //Serial.print(Ethernet.localIP()[thisByte], DEC);
    //Serial.print("."); 
    sprintf(addyPart,"%d.",Ethernet.localIP()[thisByte]);
    sprintf(remoteIPAddy,"%s%s",remoteIPAddy,addyPart);
    
  }
  myGLCD.setBackColor(0,0,0);
  myGLCD.setColor(255,255,0);
  myGLCD.print(remoteIPAddy, CENTER, 0);
  
  
}

void loop() {
  

  
  
  
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    myGLCD.setColor(0,0,0);
    myGLCD.fillRect(0,56,127,76);
    myGLCD.setColor(255,255,0);
    buf[0]='\0';
    sprintf(buf,"Rcvd pkt size %d",packetSize);
    myGLCD.print(buf,LEFT,20);
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
    //Serial.println("Contents:");
      myGLCD.print("Contents: ", CENTER, 56);
    //Serial.println(packetBuffer);
      myGLCD.print(packetBuffer, CENTER, 66);

    // send a reply, to the IP address and port that sent us the packet we received
    //Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    //Udp.write(ReplyBuffer);
    //Udp.endPacket();
  }
  delay(10);
}

