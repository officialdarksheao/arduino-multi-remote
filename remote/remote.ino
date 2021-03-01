#include <RCSwitch.h>
#include <SPI.h>
#include <Wire.h>

#define RF433PIN 4
#define PISO_LOAD 2
#define PISO_1_CLK 7
#define PISO_2_CLK 10
#define PISO_2_CLK_IN 5
#define PISO_2_Q 6

#define RF_REPEAT_COUNT 4

RCSwitch mySwitch = RCSwitch();

bool inputStates[] = {
    false, // b1
    false, // b2
    false, // b3
    false, // b4
    false, // b5
    false, // b6
    false, // b7
    false, // b8
    false, // b9
    false, // b10
    false, // b11
    false, // b12
    false, // sw1
    false // sw2
};

static unsigned long commands[] = {
    12080133, // b1
    12080137, // b2
    12080136, // b3
    12080140, // b4
    12080139, // b5
    12080129, // b6
    12080138, // b7
    12080133, // b8
    12080132, // b9
    12080131, // b10
    12080135, // b11
    12080133, // b12
    12080133, // sw1
    12080133 // sw2
};

void detectButtonPress(){
    digitalWrite(PISO_LOAD, LOW);
    delayMicroseconds(5);
    digitalWrite(PISO_LOAD, HIGH);

    digitalWrite(PISO_1_CLK, HIGH);
    digitalWrite(A2, LOW);
    byte incomingPiso1 = shiftIn(A3, PISO_1_CLK, LSBFIRST);
    digitalWrite(A2, HIGH);
    //Serial.print("Pin States:");
    //Serial.print(incomingPiso1, BIN);

    digitalWrite(PISO_2_CLK, HIGH);
    digitalWrite(PISO_2_CLK_IN, LOW);
    byte incomingPiso2 = shiftIn(PISO_2_Q, PISO_2_CLK, LSBFIRST);
    digitalWrite(PISO_2_CLK_IN, HIGH);
    //Serial.print(" ");
    //Serial.println(incomingPiso2, BIN);

    tryToggle(7, incomingPiso2, 0); // check b1
    tryToggle(5, incomingPiso2, 1); // check b2
    tryToggle(2, incomingPiso2, 2); // check b3
    tryToggle(3, incomingPiso2, 3); // check b4
    tryToggle(4, incomingPiso1, 4); // check b5
    tryToggle(7, incomingPiso1, 5); // check b6
    tryToggle(0, incomingPiso1, 6); // check b7
    tryToggle(2, incomingPiso1, 7); // check b8
    tryToggle(5, incomingPiso1, 8); // check b9
    tryToggle(6, incomingPiso1, 9); // check b10
    tryToggle(1, incomingPiso1, 10); // check b11
    tryToggle(3, incomingPiso1, 11); // check b12
    tryToggle(6, incomingPiso2, 12); // check sw1
    tryToggle(4, incomingPiso2, 13); // check sw2
}

void tryToggle(int bitIndex, byte piso, int i){
    bool newVal = bitRead(piso, bitIndex) == 0 ? true : false;
    if (newVal != inputStates[i]){
        if (newVal == false){
            sendCommand(i);
        }
    }
    inputStates[i] = newVal;
}

void sendCommand(int i){
    Serial.print("Sending command: ");
    Serial.println(i);
    mySwitch.send(commands[i], 24);
    delay(100);
}

void setup() {
    Serial.begin(9600);

    pinMode(A3, INPUT);
    pinMode(PISO_2_Q, INPUT);

    pinMode(RF433PIN, OUTPUT);
    digitalWrite(RF433PIN, LOW);
    pinMode(PISO_LOAD, OUTPUT);
    digitalWrite(PISO_LOAD, HIGH);
    pinMode(PISO_1_CLK, OUTPUT);
    digitalWrite(PISO_1_CLK, LOW);
    pinMode(A2, OUTPUT);
    digitalWrite(A2, HIGH);
    pinMode(PISO_2_CLK, OUTPUT);
    digitalWrite(PISO_2_CLK, LOW);
    pinMode(PISO_2_CLK_IN, OUTPUT);
    digitalWrite(PISO_2_CLK_IN, HIGH);

    mySwitch.enableTransmit(RF433PIN);
    mySwitch.setRepeatTransmit(RF_REPEAT_COUNT);
    Serial.print("RF Pin: ");
    Serial.println(RF433PIN);
}

void loop() {
    detectButtonPress();
    delay(100);
}