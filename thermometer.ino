#include "Thermistor.h"
#include "Display.h"

const int REFRESH_DELAY_MS = 200;

String temperatureString;
String adcString;

void setup() {
    if(Serial.available())
    {
        Serial.begin(9600);
        while (!Serial); // await for Serial connection
    }

    Display::initialize();
}

void loop() {
    int adcValue = analogRead(A0);
    double temperature = Thermistor::smoothTempFromADC(adcValue);

    Display::printTemp(adcValue, temperature);

    delay(REFRESH_DELAY_MS);
}


