# Mavin DNAx LoadCell

Arduino library for the [Mavin DNA1 and DNA2 series load cells.](https://www.tinytronics.nl/index.php?route=product/search&search=mavin%20rs485)


## How Does it Work?

This library allows for reading and changing settings of the DNA1 and DNA2 series Mavin load cells, which operate over RS485 or RS232. This library uses a UART serial port to communicate with the sensor.


## Usage and supported functionality

See the examples folder for an example for ESP32 and en example using Arduino.

NOTE: these load cells save settings when powered down, so be carefull when changing device parameters like address and baud rate.

Supported functionality:
```cpp
// Initialize the load cell
loadCell.begin(Stream &mySerial, float multiplicationFactor, uint8_t deviceAddress); // See the table below for multiplication factor values.
```
```cpp
// Return the current weight on the load cell [g]
loadCell.readWeight(); 
```
```cpp
// Return the most recent stable weight on the load cell (so not necessarily the current weight) [g]
loadCell.readStableWeight(); 
```
```cpp
// Set the current load cell state to a value of 0g (forced)
loadCell.tareDevice();
```
```cpp
// Set the load cell communication baud rate
loadCell.setBaudRate(uint32_t baudSetting); // Supported values: 2400, 4800, 9600, 19200 (default), 38400, 57600, 115200
```
```cpp
// Reset and restart the load cell (required before changing device settings)
loadCell.resetDevice(); // Important: wait 3s after calling this function before sending new commands to the load cell
```
```cpp
// Set the load cell sampling rate [Hz]
loadCell.setSamplingRate(uint8_t rate); // Supported values: 5, 10 (default), 20, 40  
```
```cpp
// Set the load cell slave address
loadCell.setAddress(uint8_t newAddress); // Supported values: 0x11 (default) to 0x7E
```


| Load cell capacity (kg)| Multiplication factor |
| ------------- | ------------- |
| 5 | 10 |
| 20 | 1 |
| 50 | 1 |
| 200 | 0.1 |

As seen in the datasheet, other features/commands (such as modifying filter, calibration, and measuring parameters) are supported by the load cell but not (yet) by this library.  


## Installing

To install this library from GitHub to Arduino, download the library as a ZIP file from the repository, then import it into Arduino IDE.


## Compatibility

This library has been tested with an ESP32 and with an Arduino Mega, using an [M5STACK TTL to RS485 converter](https://www.tinytronics.nl/index.php?route=product/product&product_id=2309). Other MCU's are probably compatible too, though only hardware serial ports have been tested successfully. 


## License

Written by TinyTronics.

MIT license, check license.txt for more information.

All text above must be included in any redistribution.
