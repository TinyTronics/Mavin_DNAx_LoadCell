#include <Arduino.h>
#include "Mavin_DNAx_LoadCell.h"

// Define the serial port for RS485 communication
#define RX_PIN 32         // Rx pin
#define TX_PIN 26         // Tx pin
#define BAUD_RATE 19200   // Default load cell baud rate

#define ADDRESS 0x11          // Default load cell address 
#define WEIGHT_MULTIPLIER 1   // Multiplication factor corresponding to load cell capacity (see README)

// Create an instance of the load cell class using HardwareSerial
HardwareSerial RS485_SerialPort(2);
Mavin_DNAx_LoadCell loadCell;


void setup() {
    Serial.begin(115200);  // Start serial monitor
    Serial.println("Initializing Load Cell...");
    
    RS485_SerialPort.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);  // Start serial communication port
    loadCell.begin(RS485_SerialPort, WEIGHT_MULTIPLIER, ADDRESS);   // Initialize the load cell
    delay(1000);       // Wait for initialization

    loadCell.resetDevice();  
    delay(3000);       // Wait 3s after reset before changing settings
 
    // Set sampling rate (default 10Hz)
    if (loadCell.setSamplingRate(10)) {
        Serial.println("Sampling rate set.");
    } else {
        Serial.println("Failed to set sampling rate...");
    }

    // Tare the load cell
    if (loadCell.tareDevice()) {
        Serial.println("Tare complete.");
    } else {
        Serial.println("Failed to tare...");
    }

    delay(1000);
}


void loop() {
    // Read weight
    float weight = loadCell.readWeight();
    if (!isnan(weight)) {
        Serial.print("Weight: ");
        Serial.print(weight);
        Serial.println(" g");
    } else {
        Serial.println("Failed to read weight.");
    }

    delay(1000);  // Wait 1 second before reading again
}