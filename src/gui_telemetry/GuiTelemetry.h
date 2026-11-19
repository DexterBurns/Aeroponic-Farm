#ifndef GUI_TELEMETRY_H
#define GUI_TELEMETRY_H

struct GuiTelemetryData {
    const char *status = "N/A";
    const char *mode = "N/A";
    int adc = 0;
    int psi = 0;
    int mistingIntervalMs = 0;
    int mistingDurationMs = 0;
    bool solenoid1Open = false;
    bool solenoid2Open = false;
    bool solenoid3Open = false;
    bool solenoid4Open = false;
    const char *cycle = "N/A";
    bool pumpOn = false;
    int savedMistingIntervalMs = 0;
    int savedMistingDurationMs = 0;
    const char *savedMode = "N/A";
    const char *savedCycle = "N/A";
    int savedMinimumPsi = 0;
    int savedMaximumPsi = 0;
    int savedMinimumPsiAdc = 0;
    int savedMaximumPsiAdc = 0;
};

void sendGuiTelemetry(const GuiTelemetryData &data);

#endif
