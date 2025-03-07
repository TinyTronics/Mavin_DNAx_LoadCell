/*
 * Mavin_DNAx_LoadCell.cpp
 *
 *  Created on: 5 mar. 2025
 *      Author: TinyTronics
 */

#include "Mavin_DNAx_LoadCell.h"

Mavin_DNAx_LoadCell::Mavin_DNAx_LoadCell() {}

void Mavin_DNAx_LoadCell::begin(Stream &serial, float weightMultiplier, uint8_t address) { 
    serialPort = &serial;
    _address = address;
    WEIGHTFACTOR = weightMultiplier;
}

void Mavin_DNAx_LoadCell::sendCommand(uint8_t address, char command, uint8_t param) {
    uint8_t packet[5] = { address, command, param, 0x00, 0x0D };
    packet[3] = calculateChecksum(packet, 3);
    serialPort->write(packet, 5);
}

bool Mavin_DNAx_LoadCell::readResponse(uint8_t *buffer, size_t length) {
    unsigned long startTime = millis();
    while (serialPort->available() < length) {
        if (millis() - startTime > 100) return false;
    }
    serialPort->readBytes(buffer, length);

    return true;
}

float Mavin_DNAx_LoadCell::parseWeight(uint8_t *data) {
    int weight = (data[4] - 0x30) * 65536 + (data[3] - 0x30) * 4096 + 
                 (data[2] - 0x30) * 256 + (data[1] - 0x30) * 16 + (data[0] - 0x30);

    return weight / WEIGHTFACTOR;
}

float Mavin_DNAx_LoadCell::readWeight() {
    sendCommand(_address, 'B', 0x3F);
    uint8_t response[10];
    if (readResponse(response, 10)) {
        return parseWeight(response + 2);
    }
    
    return NAN;
}

float Mavin_DNAx_LoadCell::readStableWeight() {
    sendCommand(_address, 'C', 0x3F);
    uint8_t response[10];
    if (readResponse(response, 10)) {
        return parseWeight(response + 2);
    }
    return NAN;
}

bool Mavin_DNAx_LoadCell::setBaudRate(uint32_t baudSetting) {
    if (baudSetting != 2400 && baudSetting != 4800 && baudSetting != 9600 && baudSetting
     != 19200 && baudSetting != 38400 && baudSetting != 57600 && baudSetting != 115200) {
        return false;
    }
    
    uint8_t baudCode;
    switch(baudSetting) {
        case 2400:
            baudCode = 0x41;
            break;
        case 4800:
            baudCode = 0x42;
            break;
        case 9600:
            baudCode = 0x43;
            break;
        case 19200:
            baudCode = 0x44;
            break;
        case 38400:
            baudCode = 0x45;
            break;
        case 57600:
            baudCode = 0x46;
            break;
        case 115200:
            baudCode = 0x47;
            break;
        default:
            baudCode = 0x44;
    }

    sendCommand(_address, 'I', baudCode);
    uint8_t response[5];
    if (readResponse(response, 5)) {
        return true;
    }

    return false;
}

bool Mavin_DNAx_LoadCell::resetDevice() {
    sendCommand(_address, 'S', 0x41);
    return true;
}

bool Mavin_DNAx_LoadCell::tareDevice() {
    sendCommand(_address, 'R', 0x41);  // (0x41 for forced tare)
    uint8_t response[5];
    if (readResponse(response, 5)) {
        if (response[2] == 0x41) {
            return true;
        }
    }
    
    return false;
}

bool Mavin_DNAx_LoadCell::setAddress(uint8_t newAddress) {
    sendCommand(_address, 'H', newAddress);
    uint8_t response[5];
    if (readResponse(response, 5)) {
        _address = newAddress;
        return true;
    }
    return false;
}

bool Mavin_DNAx_LoadCell::setSamplingRate(uint8_t rate) {
    if (rate != 5 && rate != 10 && rate != 20 && rate != 40) {
        return false;
    }
    
    uint8_t rateCode;
    switch(rate) {
        case 5:
            rateCode = 0x41;
            break;
        case 10:
            rateCode = 0x42;
            break;
        case 20:
            rateCode = 0x43;
            break;
        case 40:
            rateCode = 0x44;
            break;
        default:
            rateCode = 0x42;
    }

    sendCommand(_address, 'E', rateCode);
    uint8_t response[5];
    if (readResponse(response, 5)) {
        if (response[2] == 0x41) {
            return true;
        }
    }

    return false;
}

uint8_t Mavin_DNAx_LoadCell::calculateChecksum(uint8_t *data, size_t length) {
    uint8_t sum = 0;
    for (size_t i = 0; i < length; i++) {
        sum += data[i];
    }
    return (sum & 0x7F) + (sum == 0x0D ? 1 : 0);
}