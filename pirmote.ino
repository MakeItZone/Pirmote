// ONLY INCLUDE H4Plugins.h in the main .ino file! (You'll get linker errors.)
#include<H4Plugins.h>
#include "network.hh"
// #include "time.hh"
#include "status.hh"
#include "pir.hh"
#include "secrets.h"

// NodeMCU has *2* LEDs(!)
// This sketch should be configured so that H4 uses the LED on the ESP8266 module
// And we use the LED connected to pin 16 on the main board for indicating our app's status
// See https://stackoverflow.com/questions/14894698/why-does-extern-const-int-n-not-work-as-expected
// for why this 'extern' is still a declaration and is valid
extern int const APP_STATUS_LED_PIN = 16;

// ESP8266 Flash button on GPIO0 - used to do a full reset
const int BUTTON_PIN = 0;

const int PIR_PIN = D6;

// set which voltage level maps to the logical "active" states
const H4GM_SENSE PIR_ACTIVE = ACTIVE_HIGH;
const H4GM_SENSE LED_ACTIVE = ACTIVE_LOW;

const int PIR_TIMEOUT = 10000;

// Setup H4
H4_USE_PLUGINS(115200,H4_Q_CAPACITY,false) // Serial baud rate, Q size, SerialCmd autostop
H4P_VerboseMessages h4vm; // enable text messages instead of error coders
H4P_GPIOManager h4gm;
H4P_WiFi h4wifi(SSID,SSIDPSK,"pirmote", onWiFiConnect, onWiFiDisconnect);
H4P_AsyncMQTT h4mqtt(MQTTSERVER,1883,"","",onMqttConnect,onMqttDisconnect);
H4P_FlasherController h4fc; // for app status
// short press -> slow blinking -> reset; long press -> fast blinking -> factory (firmware) reset
H4P_MultiFunctionButton mfnb(BUTTON_PIN, INPUT, ACTIVE_LOW, 15); // pin, mode, sense, debounce (ms)
H4P_Heartbeat h4hb; // add up-time to web ui
// see https://github.com/philbowles/h4plugins/blob/master/dead/h4tk.md
// note: the DST adjustment seems to not be correct/working
// disable getting NTP time if you devices might not be able to reach the NTP server- it'll block your app until it times out.
// H4P_Timekeeper h4tk("time.google.com","time1.google.com", -8*60, &H4P_Timekeeper::DST_USA); // -28800 = -8hrs for PST
//H4P_RemoteUpdate h4ru(); // remote update... TODO. See: dead/h4ru.md

// For debugging
//
// Heap Monitoring
// https://github.com/philbowles/h4plugins/blob/master/docs/h4hw.md
// example: https://github.com/philbowles/h4plugins/blob/master/examples/DIAGNOSTICS/H4P_HeapWarn/H4P_HeapWarn.ino
// 
// Queue Monitoring
// https://github.com/philbowles/h4plugins/blob/master/docs/h4qw.md
// example: https://github.com/philbowles/h4plugins/blob/master/examples/DIAGNOSTICS/H4P_QueueWarn/H4P_QueueWarn.ino
//
// Event Logging
// https://github.com/philbowles/h4plugins/blob/master/docs/events.md
// especially Serial Logger: https://github.com/philbowles/h4plugins/blob/master/docs/slog.md
// and  MQTT log destination: https://github.com/philbowles/h4plugins/blob/master/docs/mlog.md

void h4setup() { 
    // Note: H4 constructor starts Serial
    pinMode(APP_STATUS_LED_PIN, OUTPUT);
    Serial.printf("USER: Attempting to connect to AP: %s\n", SSID.c_str());
    Serial.print("Watching GPIO ");
    Serial.print(PIR_PIN);
    Serial.print(" ACTIVE ");
    Serial.println(PIR_ACTIVE ? "HIGH":"LOW");

    // the magic line
    h4gm.Retriggering(PIR_PIN,INPUT,PIR_ACTIVE,PIR_TIMEOUT, pirTriggerCallback);
}