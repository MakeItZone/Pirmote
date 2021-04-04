#include <H4P_WiFi.h>
#include <H4P_AsyncMQTT.h>
#include "network.hh"
#include "status.hh"
#include "pir.hh"
#include "secrets.h"

using namespace std;

extern H4P_WiFi h4wifi;
extern H4P_AsyncMQTT h4mqtt;

void onWiFiConnect(void)
{
    // do things on WiFi connect...
    // like reconnect to MQTT (not clear if MQTT auto-reconnect includes wifi failure)
    Serial.println("USER: WiFi connected!");
    Serial.printf("USER: Connected SSID is %s\n", WiFi.SSID().c_str());
    Serial.printf("USER: IP is %s\n", WiFi.localIP().toString().c_str());
    updateStatus(Status::running);
}

void onWiFiDisconnect(void)
{
    // do things on WiFi disconnect...
    // like shut down mqtt connections?
    Serial.println("USER: WiFi dis-connected!");
    updateStatus(Status::disconnected);
}

// Note, H4P has some utility functions for retrieving integers from topics and payloads
// (int PARAM(n), and int H4PAYLOAD_INT - see H4P_MQTT_Wildcards.ino in the examples for details.)
uint32_t myMqttCallback(vector<string> vs)
{
    // place holder
    Serial.printf("USER: bad payload!\n");
    return H4_CMD_PAYLOAD_FORMAT;
}

void onMqttConnect()
{
    Serial.print("USER: MQTT connected\n");
    // add any desired subscription topics
    //h4mqtt.subscribeDevice("topic", myMqttCallback);
    updatePirMQTT();
}

void onMqttDisconnect()
{
    Serial.print("USER: MQTT disconnected\n");
    // don't forget to unsubscribe from any topics subscribed to in connect
    //h4mqtt.unsubscribeDevice("topic");
}

void mqttPublish(string topic, string payload) {
    h4mqtt.publishDevice(topic, payload);
}
