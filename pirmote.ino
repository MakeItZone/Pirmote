#include<H4Plugins.h>

//auto-start Serial @ 115200, Q size=20 
H4_USE_PLUGINS(115200,H4_Q_CAPACITY,false) // Serial baud rate, Q size, SerialCmd autostop

H4P_GPIOManager h4gm;
H4P_FlasherController h4fc;

#define PIR_PIN D6

// set which voltage level maps to the logical "active" states
#define PIR_ACTIVE ACTIVE_HIGH 
#define LED_ACTIVE ACTIVE_LOW

#define PIR_TIMEOUT       10000

void h4setup() { // H4 constructor starts Serial
    Serial.println("H4P_GPIOManager Retriggering Example v"H4P_VERSION);
    Serial.print("GPIO ");
    Serial.print(PIR_PIN);
    Serial.print(" ACTIVE ");
    Serial.println(PIR_ACTIVE ? "HIGH":"LOW");

    h4gm.Output(LED_BUILTIN,LED_ACTIVE,OFF);

    h4gm.Retriggering(PIR_PIN,INPUT,PIR_ACTIVE,PIR_TIMEOUT,[](H4GPIOPin* ptr){
        H4GM_PIN(Retriggering); // Create the correct pointer type in 'pin'
        Serial.print("T=");
        Serial.print(millis());
        Serial.print(" Retriggering ");
        Serial.println(pin->state ? "ON":"OFF");
        h4gm.logicalWrite(LED_BUILTIN,pin->state);
    });
}