// Motor Debug Header Files
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

int dayNightMistingCycle(int, int, bool);
void checkPressureAndRepressurize();
void buzzerAlert(int buzzes);

#endif