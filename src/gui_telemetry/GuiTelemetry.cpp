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

    char json[512];
    if (measureJson(message) >= sizeof(json)) {
        Serial.println("{\"Status\":\"Telemetry serialization overflow\"}");
        return;
    }

    size_t length = serializeJson(message, json, sizeof(json) - 1);
    if (length == 0) {
        Serial.println("{\"Status\":\"Telemetry serialization failed\"}");
        return;
    }

    json[length++] = '\n';
    Serial.write(reinterpret_cast<const uint8_t *>(json), length);
}

void sendGuiCommandAck(bool accepted, const char *messageText) {
    JsonDocument message;
    message["type"] = "command_ack";
    message["accepted"] = accepted;
    message["message"] = messageText;

    char json[256];
    if (measureJson(message) >= sizeof(json)) {
        Serial.println("{\"type\":\"command_ack\",\"accepted\":false,\"message\":\"Acknowledgment too large\"}");
        return;
    }

    size_t length = serializeJson(message, json, sizeof(json) - 1);
    if (length == 0) {
        Serial.println("{\"type\":\"command_ack\",\"accepted\":false,\"message\":\"Acknowledgment serialization failed\"}");
        return;
    }

    json[length++] = '\n';
    Serial.write(reinterpret_cast<const uint8_t *>(json), length);
}
