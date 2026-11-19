#include <Arduino.h>
#include <ArduinoJson.h>
#include <cstring>
#include "GuiCommands.h"

namespace {

bool matches(JsonVariantConst value, const char *expected) {
    return value.is<const char *>() &&
           strcmp(value.as<const char *>(), expected) == 0;
}

bool readInteger(JsonObjectConst settings, const char *key, int minimum, int maximum, int &result) {
    JsonVariantConst value = settings[key];
    if (!value.is<int>()) {
        return false;
    }

    const int parsed = value.as<int>();
    if (parsed < minimum || parsed > maximum) {
        return false;
    }

    result = parsed;
    return true;
}

}

GuiCommandParseResult parseGuiCommand(const char *json, GuiCommand &command) {
    JsonDocument document;
    if (deserializeJson(document, json)) {
        return GuiCommandParseResult::InvalidJson;
    }

    JsonVariantConst rootValue = document.as<JsonVariantConst>();
    if (!rootValue.is<JsonObjectConst>()) {
        return GuiCommandParseResult::InvalidJson;
    }

    JsonObjectConst root = rootValue.as<JsonObjectConst>();
    JsonVariantConst type = root["type"];
    JsonVariantConst parameter = root["parameter"];
    JsonVariantConst settingsValue = root["settings"];
    if (!settingsValue.is<JsonObjectConst>()) {
        return GuiCommandParseResult::InvalidSettings;
    }

    JsonObjectConst settings = settingsValue.as<JsonObjectConst>();
    GuiCommand parsedCommand;

    if (matches(type, "auto_mode") && matches(parameter, "update_auto_mode")) {
        JsonVariantConst dayNight = settings["day_night"];
        JsonVariantConst saveToMemory = settings["save_to_memory"];
        if (!readInteger(settings, "misting_interval", 1000, 6000, parsedCommand.mistingIntervalMs) ||
            !readInteger(settings, "misting_duration", 1000, 6000, parsedCommand.mistingDurationMs) ||
            !saveToMemory.is<bool>()) {
            return GuiCommandParseResult::InvalidSettings;
        }

        if (matches(dayNight, "day")) {
            parsedCommand.dayCycle = true;
        } else if (matches(dayNight, "night")) {
            parsedCommand.dayCycle = false;
        } else {
            return GuiCommandParseResult::InvalidSettings;
        }

        parsedCommand.type = GuiCommandType::UpdateAutoMode;
        parsedCommand.saveToMemory = saveToMemory.as<bool>();
    } else if (matches(type, "command") && matches(parameter, "solenoid_open")) {
        if (!readInteger(settings, "solenoid", 1, 4, parsedCommand.solenoid) ||
            !readInteger(settings, "misting_time", 1, 10000, parsedCommand.runTimeMs)) {
            return GuiCommandParseResult::InvalidSettings;
        }
        parsedCommand.type = GuiCommandType::OpenSolenoid;
    } else if (matches(type, "command") && matches(parameter, "run_motor")) {
        if (!readInteger(settings, "run_time", 1, 10000, parsedCommand.runTimeMs)) {
            return GuiCommandParseResult::InvalidSettings;
        }
        parsedCommand.type = GuiCommandType::RunMotor;
    } else if (matches(type, "calibration") && matches(parameter, "calibrate_PSI")) {
        if (!readInteger(settings, "start time", 0, 60000, parsedCommand.calibrationStartDelayMs)) {
            return GuiCommandParseResult::InvalidSettings;
        }
        parsedCommand.type = GuiCommandType::CalibratePressure;
    } else {
        return GuiCommandParseResult::UnsupportedCommand;
    }

    command = parsedCommand;
    return GuiCommandParseResult::Success;
}
