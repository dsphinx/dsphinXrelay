/**
    Copyright (c) 2017, dsphinx@plug.gr
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
     3. All advertising materials mentioning features or use of this software
        must display the following acknowledgement:
        This product includes software developed by the dsphinx.
     4. Neither the name of the dsphinx nor the
        names of its contributors may be used to endorse or promote products

    Created by Constantinos on 30/6/17.

       WIFI at my needs

       WeMOS D1 R2 & mini board
       VID: 1A86
       PID: 7523

       depends:
       dsphinX Arduino Library / ESP8266WiFi library
*/

bool debug = false;

// from globals.h
#define WiFi_ssid                         "mySSID"
#define WiFi_pass                         "myPASS"
#define WiFi_ClientName                   "HomeRelay"     // optional sensor name - NO SPACES
 
#define CollectionRelayDATA_Server_IP          "192.168.70.69"
#define CollectionRelayDATA_Server_PORT         80
#define CollectionRelayDATA_Server_Script       "/Arduino/Collector/relay.php"
#define CollectionDATA_Interval_Seconds         0   //  0 -> disable   - 11 min

//  board specific
#define RELAYPIN  4       //  4 ή 5
 
// comment out to disable blinking
#define LED_OUTPUT  14     // output LED //  comment out to disable blinking

#include <dsphinXRelay.h>    // advanced Web Client + Web Server + WiFi Support

dsphinXRelay myRelay(true);
 
// put your setup code here, to run once:
void setup() {
 
  dsphinX wireless(false);  // as Client
  wireless.initWifi();

  delay(20);
  myRelay.start();
}

 
void loop() {
  
  myRelay.runServerRelay(); 
}

