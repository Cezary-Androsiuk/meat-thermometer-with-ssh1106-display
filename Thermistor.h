#include "USBAPI.h"
#ifndef THERMISTOR_H
#define THERMISTOR_H

#include <Arduino.h>

class Thermistor{
    // Reference resistor (in Ohms)
    static constexpr double REF_RESISTOR = 46900.0; // ~ 47k ohm

    // Thermistor parameters (typical for meat probes)
    // Nominal Resistance (resistance in 25°C)
    static constexpr double NOMINAL_RESISTANCE = 48000.0; // assumption that im my room is 25°C
    // Nominal Temperature (allways 25°C for R25)
    static constexpr double NOMINAL_TEMP_C = 25.0;
    // Thermistor BETA factor
    static constexpr double BETA_COEFFICIENT = 3415.0;
    // -------------------------------------------------


    // Supply voltage
    static constexpr double VIN = 5.0;

    // Convert nominal temperature to Kelvins (for computations)
    static constexpr double NOMINAL_TEMP_K = NOMINAL_TEMP_C + 273.15;

    
    // Additions
    // smooth difference between last changes - prevents rapid flicking between values
    static constexpr int AMOUNT_OF_SMOOTHNES = 3;
    static double lastTemperatures[AMOUNT_OF_SMOOTHNES];
    static int activeValues;

public:

    // prevents rapid flicking between values
    static double smoothTempFromADC(int adcValue) 
    {
        double newTemperature = Thermistor::tempFromADC(adcValue);
        return smoothOutTemperature(newTemperature);
    }

    static double tempFromADC(int adcValue) 
    {
        double R_thermistor; // measured resistance

        // Formula assumes that layout looks like this: 5V --- [R_ref] --- (ADC) --- [Termistor] --- GND
        R_thermistor = REF_RESISTOR * (1.0 / ((1023.0 / (double)adcValue) - 1.0));

        // If layout is reversed ( 5V --- [Termistor] --- (ADC) --- [R_ref] --- GND ):
        // R_thermistor = REF_RESISTOR * ((1023.0 / (double)adcValue) - 1.0);


        // --- Krok 2: (formula Steinhart-Hart BETA) ---
        // 1/T = 1/T0 + (1/B) * ln(R/R0)

        double steinhart;
        steinhart = R_thermistor / NOMINAL_RESISTANCE;  // R/R0
        steinhart = log(steinhart);                     // ln(R/R0)
        steinhart = steinhart / BETA_COEFFICIENT;       // (1/B) * ln(R/R0)
        steinhart = steinhart + (1.0 / NOMINAL_TEMP_K); // 1/T0 + ...

        double tempK = 1.0 / steinhart; // T (Kelvin)
        double tempC = tempK - 273.15;  // T (Celcius)

        return tempC;
    }

private:
    static double smoothOutTemperature(double newTemperature)
    {
        // shift all temperatures left and add X at the end
        // [a] [b] [c] [d] [e] -> [b] [c] [d] [e] [X]
        for(int i=0; i<AMOUNT_OF_SMOOTHNES; i++)
        {
            if(i < AMOUNT_OF_SMOOTHNES-1)
                lastTemperatures[i] = lastTemperatures[i+1];
            else
                lastTemperatures[i] = newTemperature;
        }
        

        // at program start empty array need to be handled by controling the amount
        // just added value
        if(activeValues < AMOUNT_OF_SMOOTHNES)
            activeValues++;
        
        double avgSum = 0;
        for(int i=0; i<activeValues; i++)
        {
            avgSum += lastTemperatures[AMOUNT_OF_SMOOTHNES -1 -i]; // from the back
        }
        Serial.println(avgSum);
        Serial.println(activeValues);
        Serial.println();
        return avgSum / activeValues;
    }

};
int Thermistor::activeValues = 0;
double Thermistor::lastTemperatures[AMOUNT_OF_SMOOTHNES];


#endif // THERMISTOR_H