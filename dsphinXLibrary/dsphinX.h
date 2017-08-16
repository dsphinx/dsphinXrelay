/**
 *  Copyright (c) 2017, dsphinx@plug.gr
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *   3. All advertising materials mentioning features or use of this software
 *      must display the following acknowledgement:
 *      This product includes software developed by the dsphinx.
 *   4. Neither the name of the dsphinx nor the
 *      names of its contributors may be used to endorse or promote products
 *
 *  Created by Constantinos on 29/6/17.
 *
 *     WIFI at my needs
 *
 *     WeMOS D1 R2 & mini board
 *     VID: 1A86
 *     PID: 7523
 *
 *     depends:
 *     #include <ESP8266WiFi.h>
 */
#include <ESP8266WiFi.h>

#ifndef dsphinX_ARD

#include "globals.h"


class dsphinX {

private:
    unsigned char version = 1;
    bool modeAP = false;


    // is AP mode running
    bool isAPMode() {
        return (modeAP);
    }

protected:
    IPAddress IotIP;

public:
    // constructor
    dsphinX() {

        initSerial();
    }

    dsphinX(bool AP) {

        initSerial();
        modeAP = (AP == true) ? true : false;
    }

    /**
     *   init serial console
     */
    void initSerial() {

        Serial.begin(115200); //Start communication between the ESP8266-12E and the monitor window
        delay(10);
#ifdef LED_OUTPUT
        pinMode(LED_OUTPUT, OUTPUT);
#endif
    }

    /**
     *   init wifi
     */
    void initWifi() {
        initWiFi_As_Client();
        status();
    }


    /**
     *   display console messages
     *
     * @param message
     */
    void show(String message) {
        if (::debug) {
            Serial.println(message);
        }
    }

    static void showStatic(String message) {
        if (::debug) {
            Serial.println(message);
        }
    }

    /*
     *   Create new wifi SSID:  Valar_IOT  with pass: valarnet
     */
    void initWiFi_As_AccessPoint() {

        if (isAPMode()) {
            show(" Access Point function ");
            WiFi.mode(WIFI_AP); //Our ESP82.... is an AccessPoint
            WiFi.softAP(WiFi_ssid, WiFi_pass); // Provide the (SSID, password);
        } else { //  connect as client
            initWiFi_As_Client();
        }
    }


    /**
     *   Connect as Client , WPA or WEP - auto
     */
    void initWiFi_As_Client() {

        if (!isAPMode()) {

            show(" Client Function ");
            show((String) WiFi.status());
            /**
             *   Capable as Client and as AP together ..... if  WiFi.enableAP(1)
             */
            WiFi.enableAP(0);  // disable AP ESP_XXXXXX as 2 SSID

            WiFi.begin(WiFi_ssid, WiFi_pass);

            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
                show("Waiting to connect ...");
            }
            show("");
            show("WiFi connected");


        } else {
            initWiFi_As_AccessPoint();
        }


    }


    /**
     * @return  IP
     */
    IPAddress getIP() {

        if (isAPMode()) {
            IotIP = WiFi.softAPIP(); // Obtain the IP of the Server
        } else {
            IotIP = WiFi.localIP(); // Obtain the IP of the Server
        }
        return IotIP;
    }


    void status() {

        getIP(); // Obtain the IP of the Server
        Serial.println(":");
        Serial.print("Wifi IP : "); // Print the IP to the monitor window
        Serial.println(IotIP);
    }


    /**
     *
     *  if #defined  LED_OUTPUT
     *     then LED is pinmode output
     *
     * @param state
     */
    static void showLEDOutput(bool state = true, int LED = LED_OUTPUT) {

//#ifdef LED_OUTPUT
        if (state) {
            digitalWrite(LED, HIGH);
        } else {
            digitalWrite(LED, LOW);
        }
//#endif

    }

};


#define dsphinX_ARD

#endif

//
//dsphinX wi(false);  // CLient
//wi.initWiFi_As_Client();
//wi.status();
//
//dsphinX wi(true);  // AP
//
//wi.initWiFi_As_AccessPoint();
//wi.status();
//
//dsphinX wireless(false);  // Client
//wireless.initWifi();
