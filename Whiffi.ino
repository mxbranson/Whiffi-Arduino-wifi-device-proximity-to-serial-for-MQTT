/* by Michael X Branson to act as a buffered serial feed of 
 * visible wifi devices for delivery to a MQTT server.
 * Based on M. Ray Burnette's 'Sniffing6' which is based on a bunch of others :)
 * 
 * CSV serial: MAC,AP SSID,AP MAC,CHANNEL,RSSI\r\n
 * 
 * Working Summary:
 * ESP is set into promiscious mode.
 * Beacons are detected and decoded into Clients and Access Points.
 * On every new client device detection, client info is sent out via serial.
 * Devices are buffered in structure array (clientinfo ci).
 * Every interval (10,000 ms), structure array is reset forcing devices to be rediscovered and retransmitted.
 * This is to keep a serial stream of active devices nearby flowing for MQTT to publish.
 * MQTT subscribers can then watch for specific devices and proximity (RSSI value) to act on.
 * Subscribers will need to track devices and create timeouts for any triggering when a device leaves the area.
 * 
*/
#include <ESP8266WiFi.h>
#include "./functions.h"

#define disable 0
#define enable  1

long interval = 10000;           // interval at which to reset known (milliseconds)

long previousMillis = 0; 

// uint8_t channel = 1;
unsigned int channel = 1;

void setup() {
  Serial.begin(57600);
  //Serial.printf("\n\nSDK version:%s\n\r", system_get_sdk_version());
  //Serial.println(F("ESP8266 mini-sniff by Ray Burnette http://www.hackster.io/rayburne/projects"));
  //Serial.println(F("Type:   /-------MAC------/-----WiFi Access Point SSID-----/  /----MAC---/  Chnl  RSSI"));
  Serial.println();
  
  wifi_set_opmode(STATION_MODE);            // Promiscuous works only with station mode
  wifi_set_channel(channel);
  wifi_promiscuous_enable(disable);
  wifi_set_promiscuous_rx_cb(promisc_cb);   // Set up promiscuous callback
  wifi_promiscuous_enable(enable);
}

void loop() {
  channel = 1;
  wifi_set_channel(channel);
  while (true) {
    nothing_new++;                          // Array is not finite, check bounds and adjust if required
    if (nothing_new > 200) {
      nothing_new = 0;
      channel++;
      if (channel == 15) break;             // Only scan channels 1 to 14
      wifi_set_channel(channel);
    }
    delay(1);  // critical processing timeslice for NONOS SDK! No delay(0) yield()
    // Press keyboard ENTER in console with NL active to repaint the screen
    /*
    if ((Serial.available() > 0) && (Serial.read() == '\n')) {
      Serial.println("\n-------------------------------------------------------------------------------------\n");
      for (int u = 0; u < clients_known_count; u++) print_client(clients_known[u]);
     // for (int u = 0; u < aps_known_count; u++) print_beacon(aps_known[u]);
      Serial.println("\n-------------------------------------------------------------------------------------\n");
    }
    */

   // Reset known devices every 10 seconds to re-buffer MQTT broadcasts
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis > interval) {
      previousMillis = currentMillis;   
      clients_known_count = 0;
    }
  }

}

