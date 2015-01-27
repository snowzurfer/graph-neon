#ifndef _APP_GLOBALS_S
#define _APP_GLOBALS_S

// Declare the constant milliseconds per update
const int   kUpdatesPerSecond = 120;
const float kSecPerUpdate = 1.f / (float)kUpdatesPerSecond;
const float kInvSecPerUpdate = 1 / kSecPerUpdate;
const float kMSecPerUpdate = kSecPerUpdate * 1000.f;

#endif