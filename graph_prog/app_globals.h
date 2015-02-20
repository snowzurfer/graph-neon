#ifndef _APP_GLOBALS_S
#define _APP_GLOBALS_S

// Declare the constant milliseconds per update
const int   kUpdatesPerSecond = 60;
const float kSecPerUpdate = 1.f / (float)kUpdatesPerSecond;
const float kInvSecPerUpdate = 1 / kSecPerUpdate;
const float kMSecPerUpdate = kSecPerUpdate * 1000.f;
const float kPi = 3.14159265358979323846f;
const float kPiOver180 = kPi / 180.f;
const float k180OverPi = 180.f / kPi;

#endif