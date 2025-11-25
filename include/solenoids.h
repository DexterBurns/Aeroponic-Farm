// Motor Debug Header Files
#ifndef SOLENOIDS_H
#define SOLENOIDS_H

#include <Arduino.h>

void initSolenoids();
void setSolenoid(int, int);
void setSolenoid_nodelay(int);
void activateAllSolenoids(int);
void activate1by1Solenoids(int);
void deactivateAllSolenoids();

#endif