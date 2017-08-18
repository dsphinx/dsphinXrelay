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
*/

#define dsphinXVersion     .69

// save last stages
#define HistoryValues       5

/*  Access Point local
 *
#define WiFi_ssid  "Valar_IOT"
#define WiFi_pass  "valarnet"
 */
#ifndef SENSOR_DELAY_READ_ms

#define SENSOR_DELAY_READ_ms     2100

#endif
/*
 *    connect as client
 */
#ifndef WiFi_ssid

#define WiFi_ssid "ssid Wifi "
#define WiFi_pass "pass"
#endif

#ifndef WiFi_ClientName
#define WiFi_ClientName                   " Sensor DHT11"
#endif
/*
 *   Web TCP Port
 */
#define HTTPPort    80

/*
 *   Web Client
 */
#ifndef CollectionDATA_Server_IP
#define CollectionDATA_Server_IP          "192.168.70.69"
#define CollectionDATA_Server_PORT         80
#define CollectionDATA_Server_Script      "/Arduino/Collector/sensor_DHT11.php"
#endif
// CollectionDATA_Interval_Seconds = 0  stop updating
//#define CollectionDATA_Interval_Seconds    120   //  2 min
#ifndef CollectionDATA_Interval_Seconds
#define CollectionDATA_Interval_Seconds      660   //  11 min
#endif
/*
 *  global vars
 *
 */
#ifdef debug
 bool debug = true;
#endif

// do not edit below

#ifdef HistoryValues
float HistoryValuesAre[HistoryValues][1];
#endif

unsigned char HistoryValuesCurrent = 0;

