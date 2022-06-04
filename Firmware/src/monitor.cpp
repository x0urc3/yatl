#include <Arduino.h>
#include <SoftwareSerial.h>

#define BAUDRATE 9600
#define rxPin 2 
#define txPin 3 
#define resetPin 10 //ArduinoISP use pin 10 to reset 
#define ledPin 13

SoftwareSerial swSerial(rxPin, txPin); // RX, TX
bool printable = false;
bool ascii = true;

void resetSlave() {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
    for (int i = 0; i<3; i++) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
    }
    digitalWrite(resetPin, HIGH);
}

void setup() {
    char buff[30];

    Serial.begin(BAUDRATE);
    swSerial.begin(BAUDRATE);
    Serial.println("||| Serial Monitor started...");
    sprintf(buff,"||| Listening on pin RX:%d TX:%d", rxPin, txPin);
    Serial.println(buff);
    sprintf(buff,"||| Reset pin:%d | Reset slave: r", resetPin);
    Serial.println(buff);
    sprintf(buff,"||| Printable:%d | Toggle printable: p", printable);
    Serial.println(buff);
    sprintf(buff,"||| ASCII:%d | Toggle ASCII: a", ascii);
    Serial.println(buff);

    pinMode(ledPin, OUTPUT);
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, HIGH);
}

void mon_print(int ascii, char c) {
    char cc;

    if (ascii) {
        Serial.print(c);
    } else {
            sprintf(cc,"%02X",c);
            Serial.println(cc);
    }
}
void loop() {
    char buff[30];
    char c = 0;
    int cnt_unprintable = 0;

    if (Serial.available()>0) {
        c = Serial.read();
        switch (c) {
            case 'r':
                sprintf(buff,"||| Resetting pin:%d", resetPin);
                Serial.println(buff);
                resetSlave();
                break;
            case 'p':
                printable ^= 1;
                sprintf(buff,"||| Printable:%d", printable);
                Serial.println(buff);
                break;
            case 'a':
                ascii ^= 1;
                sprintf(buff,"||| ASCII:%d", printable);
                Serial.println(buff);
                break;
            default:
                break;
        }
    }

    if (swSerial.available()>0) {
        c = swSerial.read();

        if (printable) {
            if (isPrintable(c)) {
                mon_print(ascii, c);
            } 
        } else {
            mon_print(ascii, c);
            cnt_unprintable += 1;
        }

    }

    if (!printable and cnt_unprintable > 20){
        cnt_unprintable = 0;
        sprintf(buff,"||| Found %d non-printable character", cnt_unprintable);
        Serial.println(buff);
    }
}
