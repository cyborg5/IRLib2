/*
 * IR IOT demo program
 * by Chris Young
 * use with Adafruit M0 Wi-Fi
 */

#include <SPI.h>
#include <WiFi101.h>
#include "WiFi101_Util.h"

#include <IRLibSendBase.h>            // First include the send base
#include <IRLib_P01_NEC.h>            // Now include only the protocols you wish
#include <IRLib_P02_Sony.h>           // to actually use. The lowest numbered
#include <IRLib_P05_Panasonic_Old.h>  // must be first but others can be any order.
#include <IRLib_P07_NECx.h>  
#include <IRLibCombo.h>               // After all protocols, include this

IRsend My_Sender;

void processIR(WiFiClient client) {
  int protocol = client.parseInt();
  if (client.read() != '/') return; //Need more. If not there then ignore
  unsigned long code= client.parseInt();
  //If next character is a '/' then we will parse number of bits
  //otherwise assume bits are zero
  int bits =0; 
  if (client.read() == '/') {
    bits = client.parseInt();
  }
  client.print("{\"command\":\"irsend\"");
  client.print(",\"protocol\":"); client.print(protocol);
  client.print(",\"code\":");  client.print(code);
  client.print(",\"bits\":");  client.print(bits);
  client.println('}');
  My_Sender.send(protocol, code, bits);   
}

void setup() {
  WiFi101_Setup(); //moved all the setup code to a separate tab for clarity
}

void loop() {
  // listen for incoming clients
  WiFiClient client = server.available();
  if(client.available()) {
    //char c = client.read();//skip initial "/"
    #if(0) //use for debugging
      String command=client.readString ();
      Serial.print('"');Serial.print(command);Serial.print('"');
    #else 
      String command= client.readStringUntil('/');//Skips over "Put" to the start of the commands
      command= client.readStringUntil('/');
      if(command == "irsend") { //is this IR send command
        processIR(client);
      } //add an else if there are other kinds of commands
    #endif
    client.stop();
  }
  delay(50);//Poll every 50ms
}


