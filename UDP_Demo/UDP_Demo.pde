 //Processing sketch to run with this example
 //=====================================================
 
 // Processing UDP example to send and receive string data from Arduino 
 // press any key to send arrow directional commands to the arduino
 
 
 import hypermedia.net.*;
 
 UDP udp;  // define the UDP object
 
 
 void setup() {
 udp = new UDP( this, 6000 );  // create a new datagram connection on port 6000
 //udp.log( true );         // <-- printout the connection activity
 udp.listen( true );           // and wait for incoming message  
 }
 
 void draw()
 {
 }
 
 void keyPressed() {
 String ip       = "10.0.0.13"; // the remote IP address
 int port        = 8888;        // the destination port
 
 
 switch (keyCode) {
    case 37: // Left
       udp.send("Left Arrow", ip, port );   // the message to send
      break;  
    case 39:  // Right
      udp.send("Right Arrow", ip, port );   // the message to send
      break; 
    case 38: // Up
      udp.send("Up Arrow", ip, port );   // the message to send
      break;  
    case 40:  // Down
      udp.send("Down Arrow", ip, port );   // the message to send
      break; 
    default: 
       udp.send("Other Key", ip, port );   // the message to send

      break;



  }
 
 }
 
 void receive( byte[] data ) {          // <-- default handler
 //void receive( byte[] data, String ip, int port ) {   // <-- extended handler
 
 for(int i=0; i < data.length; i++) 
 print(char(data[i]));  
 println();   
 }
