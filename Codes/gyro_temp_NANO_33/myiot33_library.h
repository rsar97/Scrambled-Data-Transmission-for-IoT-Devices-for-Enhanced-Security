#ifndef MYIOT33_LIBRARY_H
#define MYIOT33_LIBRARY_H
#endif

#include <Arduino.h>


int addTwoInts(int a, int b);

void iot33StartOLED(int oledLibrary);
void displayTextOLED(String oledline[9], int oledLibrary);

void iot33StartOLED_Ascii(void);
void displayTextOLED_Ascii(String oledline[9]);
void iot33StartOLED_Adafruit(void);
void displayTextOLED_Adafruit(String oledline[9]);

void convHHMMSS(unsigned long currSeconds, char *uptimeHHMMSS);
void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS);
void convCurrentTimeET(unsigned long currSeconds, char *currentTimeET);

void getWiFiRSSI(char *wifiRSSI);
void getMacWifiShield(char *macWifiShield);
void getMacRouter(char *macRouter);
void getMacWifiShieldMacRouterSS(char *macCombined);
