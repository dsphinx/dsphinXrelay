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
 *  Created by Constantinos on 8/8/17.
 *  Filename : dsphinXRelay.h
 * 
*/
#include "dsphinX.h"
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//  WeMos - 3.3v use always -- capable of 3.3v
//  RElay 3.3v , GND , pin 5 = D15 = GPIO5


#ifndef DSPHINX_DSPHINXRELAY_H
#define DSPHINX_DSPHINXRELAY_H

ESP8266WebServer dsphinXRelayWeb(HTTPPort);

bool RelayState = false;
unsigned int dsphinXRelayLastTriggerDigit = 0;
String RelayStateChanged = "";
int RELAY_PIN_NO = 4;       // default
WiFiClient client;

class dsphinXRelay : public dsphinX {

private:


    /**
     *
     *    return proper HTML5 header
     *
     * @param title
     * @param style
     * @param JS
     * @return
     */
    static String htmlHeader(String title = "", String style = "", String JS = "", String ExtraMeta = "") {

        return "<!DOCTYPE html>\
                <html>\
                <head>\
                <meta  name='author' content=' dsphinX ' />\
                <meta http-equiv='X-UA-Compatible' content='IE=edge,chrome=1' />\
                <meta charset='utf-8' />\
                " + ExtraMeta + "\
                <title>" + title + "</title>\
                <style>" + style + "</style>\
                <script type='text/javascript'>" + JS + "</script>\
               </head>\
                <body>\n";
    }

    /**
     *   proper html end
     *
     * @return
     */
    static String htmlFooter() {

        return "\n</body> \
                <!-- dsphinX -->\
                \n</html>";
    }

    /**
     *
     *   get uptime
     *
     * @return
     */
    static String uptime() {
        char temp[40];
        int sec = millis() / 1000;
        int min = sec / 60;
        int hr = min / 60;

        snprintf(temp, 400, "%02d:%02d:%02d", hr, min % 60, sec % 60);
        return (String) temp;
    }


protected:


    /**
     *   hook inline CSS
     *
     * @return
    */
    static String htmlCSS() {

        String ret = "html,body {\
		display: -webkit-box;\
		display: -ms-flexbox;\
		display: flex;\
		min-height: 100%;\
		-webkit-box-pack: center;\
		-ms-flex-pack: center;\
		justify-content: center;\
		-webkit-box-align: center;\
		-ms-flex-align: center;\
		align-items: center;\
		-webkit-box-orient: vertical;\
		-webkit-box-direction: normal;\
		-ms-flex-direction: column;\
		flex-direction: column;\
		font-family: sans-serif;\
		background: -webkit-radial-gradient(circle, #92f7fc 0%, #eef1f8 100%);\
		height: 100%;\
		width: 100%;\
	}\
	#status {\
		position: absolute;\
		bottom: 0;\
		right: 0;\
		color: red;\
	}\
	ul,\
        li {\
		list-style: none;\
		margin: 0;\
		padding: 0;\
	}\
	.tg-list {\
		text-align: center;\
		display: -webkit-box;\
		display: -ms-flexbox;\
		display: flex;\
		-webkit-box-align: center;\
		-ms-flex-align: center;\
		align-items: center;\
	}\
	.tg-list-item {\
		margin: 0 2em;\
	}\
	h2 {\
		color: #777;\
	}\
	h4 {\
		color: #999;\
	}\
	.tgl {\
		display: none;\
	}\
	.tgl, .tgl:after, .tgl:before, .tgl *, .tgl *:after, .tgl *:before, .tgl + .tgl-btn {\
		box-sizing: border-box;\
	}\
	.tgl::-moz-selection, .tgl:after::-moz-selection, .tgl:before::-moz-selection, .tgl *::-moz-selection, .tgl *:after::-moz-selection, .tgl *:before::-moz-selection, .tgl + .tgl-btn::-moz-selection {\
		background: none;\
	}\
	.tgl::selection, .tgl:after::selection, .tgl:before::selection, .tgl *::selection, .tgl *:after::selection, .tgl *:before::selection, .tgl + .tgl-btn::selection {\
		background: none;\
	}\
	.tgl + .tgl-btn {\
		outline: 0;\
		display: block;\
		width: 4em;\
		height: 2em;\
		position: relative;\
		cursor: pointer;\
		-webkit-user-select: none;\
		-moz-user-select: none;\
		-ms-user-select: none;\
		user-select: none;\
	}\
	.tgl + .tgl-btn:after, .tgl + .tgl-btn:before {\
		position: relative;\
		display: block;\
		content: "";\
		width: 50%;\
		height: 100%;\
	}\
	.tgl + .tgl-btn:after {\
		left: 0;\
	}\
	.tgl + .tgl-btn:before {\
		display: none;\
	}\
	.tgl:checked + .tgl-btn:after {\
		left: 50%;\
	}\
	.tgl-skewed + .tgl-btn {\
		overflow: hidden;\
		-webkit-transform: skew(-10deg);\
		transform: skew(-10deg);\
		-webkit-backface-visibility: hidden;\
		backface-visibility: hidden;\
		-webkit-transition: all .2s ease;\
		transition: all .2s ease;\
		font-family: sans-serif;\
		background: #888;\
	}\
	.tgl-skewed + .tgl-btn:after, .tgl-skewed + .tgl-btn:before {\
		-webkit-transform: skew(10deg);\
		transform: skew(10deg);\
		display: inline-block;\
		-webkit-transition: all .2s ease;\
		transition: all .2s ease;\
		width: 100%;\
		text-align: center;\
		position: absolute;\
		line-height: 2em;\
		font-weight: bold;\
		color: #fff;\
		text-shadow: 0 1px 0 rgba(0, 0, 0, 0.4);\
	}\
	.tgl-skewed + .tgl-btn:after {\
		left: 100%;\
		content: attr(data-tg-on);\
	}\
	.tgl-skewed + .tgl-btn:before {\
		left: 0;\
		content: attr(data-tg-off);\
	}\
	.tgl-skewed + .tgl-btn:active {\
		background: #888;\
	}\
	.tgl-skewed + .tgl-btn:active:before {\
		left: -10%;\
	}\
	.tgl-skewed:checked + .tgl-btn {\
		background: #86d993;\
	}\
	.tgl-skewed:checked + .tgl-btn:before {\
		left: -100%;\
	}\
	.tgl-skewed:checked + .tgl-btn:after {\
		left: 0;\
	}\
  .tgl-skewed:checked + .tgl-btn:active:after {\
		left: 10%;\
	}";

        return ret;
    }

    /**
     *   HTML5 localstorage
     *
     *
     * @return
     */
    static String htmlJSLocalStorage() {

        // sessionStorage.clear();

        String JS = "function toggleClass(element) {\
        var classString = element.className;\
        var isOn = element.checked;\
        var title = ' Relay Control : ';\
        var xhr = new XMLHttpRequest();\
        if (isOn) {\
            xhr.open('GET', '/ON');\
            document.title = title + ' ON '; \
        } else {\
            xhr.open('GET', '/OFF');\
            document.title = title + ' OFF '; \
        }\
        xhr.send(null);\
        }\
        document.getElementById('Button_ONOFF').addEventListener('click', function () {\
            toggleClass(document.getElementById('Button_ONOFF'));\
        });  ";


        return JS;
    }


    /**
   *   URI not found
   */
    static void handleNotFound() {
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += dsphinXRelayWeb.uri();
        message += "\nMethod: ";
        message += (dsphinXRelayWeb.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += dsphinXRelayWeb.args();
        message += "\n";
        for (uint8_t i = 0; i < dsphinXRelayWeb.args(); i++) {
            message += " " + dsphinXRelayWeb.argName(i) + ": " + dsphinXRelayWeb.arg(i) + "\n";
        }
        dsphinXRelayWeb.send(404, "text/plain", message);
    }

    /**
     *   /status
     *   /ON
     *   /OFF
     *   /update
     *
     *
     */
    static void handleStatus() {

        String showMessage = dsphinXRelay::htmlHeader("Arduino ESP8266 +  Relay + dsphinX Library Status",
                                                      dsphinXRelay::htmlCSS(), "",                                                      "");
        showMessage += " <br/> <br/><h2> Uptime <span class=\"uptime\">" + dsphinXRelay::uptime() +
                       "</span> </h2> <br/> <br/>";

        dsphinX::showLEDOutput(true);

        showMessage += "<ul class='tg-list'> <li class='tg-list-item'><a target='_blank' href='/ON'> Relay ON </a> </li><li class='tg-list-item'>";
        showMessage += "<a target='_blank' href='/OFF'> Relay OFF </a> </li><li class='tg-list-item'>";
        showMessage += "<a target='_blank' href='/getState'> Current State </a> </li><li class='tg-list-item'>";
        showMessage += "<a target='_blank' href='/scanWifi'> Wifi Scanner </a> </li> </ul>";
        showMessage += " <p> <br/><a id='status' target='_blank' href='/update'> <small> update firmware </small> </a> </p>";
        showMessage += " <p> <br/> <small> Collection Server " + (String) CollectionRelayDATA_Server_IP + " script " +
                       (String) CollectionRelayDATA_Server_Script + ", as " + (String) WiFi_ClientName + " </small>  </p>";
        showMessage += " <p> <br/> <small> firmware version " + (String) dsphinXVersion + " </small>  </p>";


        showMessage += dsphinXRelay::htmlFooter();
        dsphinXRelayWeb.send(200, "text/html", showMessage);

        dsphinX::showLEDOutput(false);
    }


    /**
   *
   *   send data to Server
   *
   * debugging won server side :
   *
   * sudo tcpdump -nl -w - -i eth0 -c 500 port 80|strings
   *
   * @param GetParams
   * @param Host
   * @param Port
   * @param Script
   * @param Proto
   * @return
   */
    static bool sendCollectionData(String GetParams="", String Host = "", String Port = "", String Script = "", String Proto = "http://") {

        String uri = "";
        bool ret = false;

        GetParams = (GetParams == "") ?"?state=" + (String)  RelayState  : GetParams;
        Host = (Host == "") ? (String) CollectionRelayDATA_Server_IP : Host;
        Port = (Port == "") ? (String) CollectionRelayDATA_Server_PORT : Port;
        Script = (Script == "") ? (String) CollectionRelayDATA_Server_Script : Script;

        uri = Proto + Host + ":" + Port + Script + GetParams + "&ver=" + (String) dsphinXVersion;

#ifdef WiFi_ClientName
        uri += "&sensorName=" + (String) WiFi_ClientName;
#endif

        if (::debug) ("URI:" + uri + " \n");

        if (!client.connect(CollectionRelayDATA_Server_IP, CollectionRelayDATA_Server_PORT)) {
            if (::debug)("connection to Server Collection FAILED :" + (String) CollectionRelayDATA_Server_IP + ":" +
                 (String) CollectionRelayDATA_Server_PORT);
        }

        // This will send the request to the server
        client.print(String("GET ") + uri + " HTTP/1.1\r\n" +
                     "Host: " + Host + "\r\n" +
                     "Connection: close\r\n\r\n");
//        client.print(uri);


        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                if (::debug)(">>> Client Timeout !");
                client.stop();
                return ret;
            }
        }


        // return ret;
        if (::debug) {
            /**
             *    Error code boot(1,6) boot(1,7) when uncommented below
             */
            // Read all the lines of the reply from server and print them to Serial
//            while (client.available()) {
//                String line = client.readStringUntil('\r');
//                if (line == "OK") {
//                    ret = true;
//                    show("RESPONSE --> " + line);
//
//                }
//                show(line);
//            }

        }

        client.stop();

        ret = true;
        if (::debug)(" request OK !");

        return ret;
    }


    /**
     *  UPDATE:
     *
     *    handle HTML FORM to update firmware image
     *
     */
    static void handleUpdate() {

        // 1st create labda function for .bin file handler
        dsphinXRelayWeb.on("/update_firmware", HTTP_POST, []() {
            dsphinXRelayWeb.sendHeader("Connection", "close");
            dsphinXRelayWeb.sendHeader("Access-Control-Allow-Origin", "*");
            dsphinXRelayWeb.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
            ESP.restart();
        }, []() {
            HTTPUpload &upload = dsphinXRelayWeb.upload();
            if (upload.status == UPLOAD_FILE_START) {
                Serial.setDebugOutput(true);
                WiFiUDP::stopAll();
                Serial.printf("Update: %s\n", upload.filename.c_str());
                uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                if (!Update.begin(maxSketchSpace)) {//start with max available size
                    Update.printError(Serial);
                }
            } else if (upload.status == UPLOAD_FILE_WRITE) {
                if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                    Update.printError(Serial);
                }
            } else if (upload.status == UPLOAD_FILE_END) {
                if (Update.end(true)) { //true to set the size to the current progress
                    Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
                } else {
                    Update.printError(Serial);
                }
                Serial.setDebugOutput(false);
            }
            delay(1);
            yield();
            delay(1);
        });


        String serverIndex = "<h1> ESP8266 Firmware Update  </h1>\
                              <h3> firmware version " + (String) dsphinXVersion + " </h3>\
                             <h3> .bin file </h3>\
                             <FORM method='POST' action='/update_firmware' enctype='multipart/form-data'>\
                             <input type='file' name='update' accept='.bin'>\
                             <input type='submit' value='Update'></FORM><br />\
                              <small id='status'>dsphinX Library 1-7-2017</small>";

        dsphinXRelayWeb.sendHeader("Connection", "close");
        dsphinXRelayWeb.sendHeader("Access-Control-Allow-Origin", "*");
        dsphinXRelayWeb.send(200, "text/html", serverIndex);

    }

    /**
   *   display info about relay
   */
    static void handleRoot() {

        String showMessage = dsphinXRelay::htmlHeader("Relay Control - dsphinX ", dsphinXRelay::htmlCSS(), ""                     ,                                                      "");

        String  whatMessage =  (RelayState) ? "false" : "true";

        showMessage +=              "<br/> <a id='status' target='_blank' href='/status'> <small> [ status ]</small> </a>";
        showMessage += "<h2>Relay Control " + (String) WiFi_ClientName +"</h2>\
                <ul class='tg-list'>\
                <li class='tg-list-item'>\
                <h4>Module Relay is   <span id='state'> </span>  </h4>\
                <input title='toggle Relay Module ' id='Button_ONOFF' class='tgl tgl-skewed' type='checkbox'/>\
                <label class='tgl-btn' data-tg-off='OFF' data-tg-on='ON' for='Button_ONOFF'></label>\
                </li>\
                </ul>";

        showMessage += dsphinXRelay::htmlFooter();
        showMessage += "<script>" +  dsphinXRelay::htmlJSLocalStorage() + "document.getElementById('Button_ONOFF').checked ="+whatMessage+";</script>";

        dsphinX::showLEDOutput(true);

        dsphinXRelayWeb.send(200, "text/html", showMessage);
// dsphinXWebServer
        dsphinX::showLEDOutput(false);

    }


    /**
     *   show near wifi
     */
    static void handleScanWifi() {

        String showMessage = dsphinXRelay::htmlHeader("dsphinX Wifi Scanner", dsphinXRelay::htmlCSS(),
                                                      dsphinXRelay::htmlJSLocalStorage(),
                                                      "<meta http-equiv='refresh' content='60'/>");
        showMessage += "Wifi Scanning ...";

        // WiFi.scanNetworks will return the number of networks found
        int n = WiFi.scanNetworks();
        showMessage += " <br/> init  ";
        if (n == 0)
            showMessage += "<br/>no networks found";
        else {
            showMessage += (String) n;
            showMessage += " networks found <br/><br/>";
            for (int i = 0; i < n; ++i) {
                // Print SSID and RSSI for each network found
                showMessage += (String)(i + 1);
                showMessage += ": ";
                showMessage += WiFi.SSID(i);
                showMessage += " (";
                showMessage += WiFi.RSSI(i);
                showMessage += ")";
                showMessage += ((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
                delay(10);
                showMessage += "<br/>------<br/>";
            }
        }
        showMessage += "<br/>";


        dsphinXRelayWeb.send(200, "text/html", showMessage);

    }


    static void getState() {

        String showMessage = (String) RelayState;

        dsphinXRelayWeb.send(200, "text/html", showMessage);

    }



public:

    dsphinXRelay(bool start = true) {

#ifdef RELAYPIN
        RELAY_PIN_NO = RELAYPIN;
#endif

        if (start) {
            // Output
            pinMode(RELAYPIN, OUTPUT);
            show(" Relay Status: Init Output : PIN = " + (String) RELAY_PIN_NO);
        }
    }


    /**
 *    init HTTP Server
 *
 * @param go
 */
    void start(String go = "simple") {

        if (go == "simple") {
            dsphinXRelayWeb.on("/", dsphinXRelay::handleRoot);
            show("simple HTTP server without   started at URI ");
            show((String) IotIP);
        }

        dsphinXRelayWeb.on("/ON", dsphinXRelay::setON);         // /on
        dsphinXRelayWeb.on("/OFF", dsphinXRelay::setOFF);       // /off
        dsphinXRelayWeb.on("/getState", dsphinXRelay::getState);       // /off
        dsphinXRelayWeb.on("/scanWifi", dsphinXRelay::handleScanWifi);     // /scanWifi

        dsphinXRelayWeb.on("/status", dsphinXRelay::handleStatus);     // /status
        dsphinXRelayWeb.on("/update", dsphinXRelay::handleUpdate);     // /update  UPDATE .bin from web
        dsphinXRelayWeb.onNotFound(dsphinXRelay::handleNotFound);      // NOT found uri, error 404

        dsphinXRelayWeb.begin();
    }

    /**
     *
     *   handle RELAY_PIN  functions
     *
     * @param status
     */
    static void handleRelay(bool status) {

        int what = (status) ? HIGH : LOW;
        String  whatMessage = (status) ? " Relay OFF " : " Relay ON ";

        String showMessage = dsphinXRelay::htmlHeader("Relay Control - dsphinX ", dsphinXRelay::htmlCSS(),
                                                      dsphinXRelay::htmlJSLocalStorage(), "");

        digitalWrite(RELAYPIN, what);

        RelayStateChanged = dsphinXRelay::uptime();


        showMessage += whatMessage;

        showMessage += dsphinXRelay::htmlFooter();

        dsphinX::showLEDOutput(true);

        dsphinXRelayWeb.send(200, "text/html", showMessage);
        // dsphinXWebServer
        dsphinX::showLEDOutput(false);

        sendCollectionData();

    }

    /**
     *   turn circuit on
     */
    static void setON() {
        RelayState = false;
        handleRelay(RelayState);
        if (::debug) {
            Serial.println(" Relay Status: ON");
        }
    }

    /**
     * turn circuit off
     */
    static void setOFF() {
        RelayState = true;
        handleRelay(RelayState);
        if (::debug) {
            Serial.println(" Relay Status: OFF ");

        }
    }

    /**
    *   run at infinity from loop()
    *
    *    handle Client
    */
    void runServerRelay() {

        dsphinXRelayWeb.handleClient();
    }

};

#endif //DSPHINX_DSPHINXRELAY_H
