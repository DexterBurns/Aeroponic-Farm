#include <Arduino.h>
#include <ArduinoJson.h>
#include "GuiTelemetry.h"

void sendGuiTelemetry(const GuiTelemetryData &data) {
    JsonDocument message;

    message["Status"] = data.status;
    message["Mode"] = data.mode;
    message["ADC"] = data.adc;
    message["PSI"] = data.psi;
    message["Misting Interval"] = data.mistingIntervalMs;
    message["Misting Duration"] = data.mistingDurationMs;
    message["Solendoid 1 Status"] = data.solenoid1Open ? "Open" : "Closed";
    message["Solendoid 2 Status"] = data.solenoid2Open ? "Open" : "Closed";
    message["Solendoid 3 Status"] = data.solenoid3Open ? "Open" : "Closed";
    message["Solendoid 4 Status"] = data.solenoid4Open ? "Open" : "Closed";
    message["Cycle"] = data.cycle;
    message["Pump Status"] = data.pumpOn ? "On" : "Off";
    message["Saved Misting Interval"] = data.savedMistingIntervalMs;
    message["Saved Misting Duration"] = data.savedMistingDurationMs;
    message["Saved Mode"] = data.savedMode;
    message["Saved Cycle"] = data.savedCycle;
    message["Saved Minimum PSI"] = data.savedMinimumPsi;
    message["Saved Maximum PSI"] = data.savedMaximumPsi;
    message["Saved Minimum PSI ADC"] = data.savedMinimumPsiAdc;
    message["Saved Maximum PSI ADC"] = data.savedMaximumPsiAdc;

    serializeJson(message, Serial);
    Serial.write('\n');
}
