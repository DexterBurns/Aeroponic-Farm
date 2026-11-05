// Motor Debug Header Files
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>
#include <Preferences.h>
void schedulerInit();
int dayNightMistingCycle(int, int, bool, Preferences &);
void checkPressureAndRepressurize();
void buzzerAlert(int buzzes);

#endif