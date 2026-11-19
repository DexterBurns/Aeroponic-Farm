#ifndef GUI_COMMANDS_H
#define GUI_COMMANDS_H

enum class GuiCommandType {
    UpdateAutoMode,
    OpenSolenoid,
    RunMotor,
    CalibratePressure
};

struct GuiCommand {
    GuiCommandType type = GuiCommandType::UpdateAutoMode;
    int mistingIntervalMs = 0;
    int mistingDurationMs = 0;
    bool dayCycle = true;
    bool saveToMemory = false;
    int solenoid = 0;
    int runTimeMs = 0;
    int calibrationStartDelayMs = 0;
};

enum class GuiCommandParseResult {
    Success,
    InvalidJson,
    UnsupportedCommand,
    InvalidSettings
};

GuiCommandParseResult parseGuiCommand(const char *json, GuiCommand &command);

#endif
