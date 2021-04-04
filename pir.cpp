#include "pir.hh"
#include "network.hh"

using namespace std;

extern H4P_GPIOManager h4gm; 

bool pirState = false;

void updatePirMQTT() {
    string topic = string("status");
    string status;

    if (pirState) {
        status = string("triggered");
    } else {
        status = string("clear");
    }
    mqttPublish(topic, status);
}

void pirTriggerCallback(H4GPIOPin* ptr){
    H4GM_PIN(Retriggering); // Create the correct pointer type in 'pin'
    Serial.print("T=");
    Serial.print(millis());
    Serial.print(" Retriggering ");
    Serial.println(pin->state ? "ON":"OFF");
    h4gm.logicalWrite(LED_BUILTIN,pin->state);
    pirState = pin->state;
    updatePirMQTT();
}
