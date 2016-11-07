/**
 * LED Strip Fire Simulator
 *
 * Drives an addressable LED strip to generate a simulated fire lighting effect.
 * Uses the homie-esp8266 to connect to an MQTT broker for remote control and
 * status monitoring.  A companion project provides a button panel to remotely
 * control the simulator, or it can be controlled via MQTT.
 */

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <Homie.h>

#define VOLTAGE_PIN A0

bool reported = false;
HomieNode batteryNode("battery", "voltage");

HomieSetting<double> voltageCoefficientSetting("voltage_coefficient", "ADC scaling factor for battery voltage");

/**
 * Report the battery voltage.
 */
void reportVoltage()
{
    int voltageCount = analogRead(VOLTAGE_PIN);
    float voltage = voltageCoefficientSetting.get() * voltageCount;
    batteryNode.setProperty("voltage").send(String(voltage));
}

/**
 * Called once when Homie is connected and ready.
 */
void setupHandler()
{

}

/**
 * Looped while Homie is operating.
 */
void loopHandler()
{
  if( ! reported ) {
    reportVoltage();

    reported = true;
  }
}

/**
 * Called when Homie transitions between states
 */
void eventHandler(HomieEvent event) {
}

void setup()
{
  Serial.begin(115200);

  Homie_setFirmware("firelight", "1.0.10");
  Homie_setBrand("clough42");

  Homie
    .disableResetTrigger()
    .setSetupFunction(setupHandler)
    .onEvent(eventHandler)
    .setLoopFunction(loopHandler)
    .setup();
}

void loop()
{
  Homie.loop();
}
