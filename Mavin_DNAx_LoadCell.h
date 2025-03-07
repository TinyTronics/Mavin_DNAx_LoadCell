/*
 * Mavin_DNAx_LoadCell.h
 *
 *  Created on: 5 mar. 2025
 *      Author: TinyTronics
 */

#ifndef MAVIN_DNAX_LOADCELL_H
#define MAVIN_DNAX_LOADCELL_H

#include <Arduino.h>
#include <Stream.h>

class Mavin_DNAx_LoadCell {
public:
Mavin_DNAx_LoadCell();
    void begin(Stream &serial, float weightMultiplier = 1, uint8_t address = 0x11);
    float readWeight();
    float readStableWeight();
    bool tareDevice();
    bool setBaudRate(uint32_t baudSetting);
    bool resetDevice();
    bool setSamplingRate(uint8_t rate);
    bool setAddress(uint8_t newAddress);
    
private:
    Stream *serialPort;
    uint8_t _address;
    float WEIGHTFACTOR;
    void sendCommand(uint8_t address, char command, uint8_t param);
    bool readResponse(uint8_t *buffer, size_t length);
    float parseWeight(uint8_t *data);
    uint8_t calculateChecksum(uint8_t *data, size_t length);
};

#endif