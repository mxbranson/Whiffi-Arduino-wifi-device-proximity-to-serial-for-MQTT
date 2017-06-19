# Whiffi
Turn an ESP-01 into a wifi device proximity serial feed for publishing to MQTT or other fun uses ;)

 
 
## Output:
 * CSV serial: MAC,AP SSID,AP MAC,CHANNEL,RSSI\r\n

## Working Summary:
 * ESP is set into promiscuous mode.
 * Beacons are detected and decoded into Clients and Access Points.
 * On every new client device detection, client info is sent out via serial.
 * Devices are buffered in structure array (clientinfo ci).
 * Every interval (10,000 ms), structure array is reset forcing devices to be rediscovered and retransmitted.
 * This is to keep a serial stream of active devices nearby flowing for MQTT to publish.
 * MQTT subscribers can then watch for specific devices and proximity (RSSI value) to act on.
 * Subscribers will need to track devices and create timeouts for any triggering when a device leaves the area.

 
  Compiled sucessfully using Arduino 1.8.0 to an ESP-01
    Sketch uses 226853 bytes (52%) of program storage space. Maximum is 434160 bytes.
    Global variables use 45092 bytes (55%) of dynamic memory, leaving 36828 bytes for local variables. Maximum is 81920 bytes.
    
    
 History:
 based on RandDruid/esp8266-deauth (MIT) https://github.com/RandDruid/esp8266-deauth
 
 inspired by kripthor/WiFiBeaconJam (no license) https://github.com/kripthor/WiFiBeaconJam
 
 https://git.schneefux.xyz/schneefux/jimmiejammer/src/master/jimmiejammer.ino
 
 requires SDK v1.3: install esp8266/Arduino from git and checkout commit 1c5751460b7988041fdc80e0f28a31464cdf97a3
 
 Modified by M. Ray Burnette for publication as WiFi Sniffer 20161013
 
 Modified further by Michael X Branson to act as a bufered serial feed of visible devices for delivery to a MQTT server.
