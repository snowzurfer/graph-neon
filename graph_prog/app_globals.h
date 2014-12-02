#ifndef _APP_GLOBALS_S
#define _APP_GLOBALS_S


const int TICKS_PER_SECOND = 40;
const float kSecondsPerUpdate = 1.f / (float)TICKS_PER_SECOND; 
const float SKIP_TICKS = 1000.f / (float)TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;


#endif