#ifndef pir_h
#define pir_h
#include<H4P_GPIOManager.h>

using namespace std;

void updatePirMQTT();

void pirTriggerCallback(H4GPIOPin* ptr);

#endif