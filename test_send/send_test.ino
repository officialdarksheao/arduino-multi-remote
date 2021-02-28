#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableTransmit(4);
  pinMode(13, OUTPUT); // D13 as output- Optional
  mySwitch.setRepeatTransmit(4);
  Serial.println("Ready to send on pin 4");
}

void loop() {
    delay(5000);
    Serial.println("Sending");
    mySwitch.send("101110000101010000000101");
}