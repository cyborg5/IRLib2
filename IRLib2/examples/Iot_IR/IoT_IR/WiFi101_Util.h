/*
 * Moved all of the Wi-Fi initialization and debugging code here for clarity
 */
#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

//Set to 1 if you are going to open the serial monitor
#define SERIAL_DEBUG 0
IPAddress ip;
int status = WL_IDLE_STATUS;
// Initialize the WiFi server library
WiFiServer server(80);

#if SERIAL_DEBUG
  void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your WiFi shield's IP address:
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
#endif

void WiFi101_Setup(void) {
  #if defined(ARDUINO_SAMD_FEATHER_M0)
    //Configure pins for Adafruit ATWINC1500 Feather
    WiFi.setPins(8,7,4,2);
  #endif
  
  #if SERIAL_DEBUG
    // Start Serial
    Serial.begin(115200);
  
    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
      Serial.println("WiFi shield not present");
      // don't continue:
      while (true);
    }
  
    String fv = WiFi.firmwareVersion();
    if ( fv != "1.1.0" )
      Serial.println("Please upgrade the firmware");
  #endif
  // Attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    #if SERIAL_DEBUG
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
    #endif
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // Wait 10 seconds for connection
    delay(10000);
  }
  // Start the server
  server.begin();
  ip = WiFi.localIP();
  #if SERIAL_DEBUG
    // Print out the status
    printWifiStatus();
  #endif
}


