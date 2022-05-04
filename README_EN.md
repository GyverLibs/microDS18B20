This is an automatic translation, may be incorrect in some places. See sources and examples!

#microDS18B20
Lightweight library for working with 1-Wire (OneWire) Dallas DS18B20 thermometers
- Work with several sensors on one pin (addressing mode)
- Storing an array of addresses in PROGMEM memory
- Work with one sensor per pin (without using addressing)
- Temperature calculation in integers and floating point
- Reading raw data for cases of strong memory savings
- Checking the correctness of the received temperature
- Customizable conversion resolution
- Data authentication
- Checking the correct operation of the sensor

### Compatibility
Compatible with all Arduino platforms (using Arduino functions)
- Does not work on Digispark, some kind of conflict with the compiler. Use the [ATTInyCore Universal](https://github.com/SpenceKonde/ATTinyCore) core with ATtiny85 + Micronucleous (Digispark) setup instead of the standard Digi core

## Content
- [Install](#install)
- [Initialization](#init)
- [Usage](#usage)
- [Connection](#wiring)
- [Examples](#examples)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- The library can be found under the name **microDS18B20** and installed through the library manager in:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Download Library](https://github.com/GyverLibs/microDS18B20/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Initialization
```cpp
// one sensor per pin without addressing
MicroDS18B20<uint8_t pin> ds;

// several sensors on a pin with addressing, specify the address (address - uint8_t array)
MicroDS18B20<uint8_t pin, uint8_t *address> ds;

// indicate that we will work with addressing. We will pass the address itself later (in setAddress())
MicroDS18B20<uint8_t pin, DS_ADDR_MODE>;

// indicate that we will work with addressing, and there will be several (amount) sensors on the line
// see async_read_many_bus example
MicroDS18B20<uint8_t pin, DS_ADDR_MODE, uint8_t amount>;

// indicate that we will work with addressing, there will be several (amount) sensors on the line, and we will store the addresses in PROGMEM
// see async_read_many_bus_pgm example
MicroDS18B20<uint8_t pin, DS_ADDR_MODE, uint8_t amount, DS_PROGMEM>;
```

<a id="usage"></a>
## Usage
```cpp
// ============= CLASS METHODS =============
bool readAddress(uint8_t *addressArray); // Read the thermometer's unique address into an array. [true if successful]
void setAddress(uint8_t *addr); // set (change) address
void setResolution(uint8_t resolution); // Set resolution 9-12 bits
bool online(); // check connection with the sensor (true - the sensor is online). Tire must be tightened

void requestTemp(); // Request a new temperature transformation
bool readTemp(); // read the temperature from the sensor. [true if successfulcranberry shno]

float getTemp(); // get temperature value in float
int16_t getTempInt(); // get temperature value in int
int16_t getRaw(); // get "raw" temperature value (16 times more than real temperature)

// ======= SENSOR BUS METHODS =======
// see async_read_many_bus and async_read_many_bus_pgm examples
void setResolutionAll(uint8_tres); // Set the resolution to 9-12 bits for all sensors on the line
void setResolution(uint8_t resolution, uint8_t idx); // Set resolution 9-12 bits (sensor idx)
bool online(uint8_t idx); // check connection (sensor idx)

void requestTempAll(); // temperature request from all sensors on the line
void requestTemp(uint8_t idx); // Request a new temperature conversion (sensor idx)
bool readTemp(uint8_t idx); // read the temperature from the sensor (sensor idx)

float getTemp(uint8_t idx); // get temperature value in float (sensor idx)
int16_t getTempInt(uint8_t idx); // get the temperature value in int (sensor idx)
int16_t getRaw(uint8_t idx); // get "raw" temperature value (sensor idx)

// =========== FUNCTIONS OUTSIDE THE CLASS ===========
int DS_rawToInt(int data); // convert raw data to temperature int
float DS_rawToFloat(int data); // convert raw data to float temperature

// ============ DEFINE SETTINGS ============
// prescribe before connecting the library
#define DS_CHECK_CRC [true / false] // Data authentication. If disabled, it will return an incorrect value on transmission failure (default true)
#define DS_CRC_USE_TABLE [true / false] // Use tablefor CRC. Faster, but +256 bytes flash (<1µs VS ~6µs) (default false)

// ================== INFO ===================
// Conversion time from precision
precision | time
12 bit | 750ms
11 bit | 375 ms
10 bit | 187 ms
9 bit | 93 ms
```

### Working with the sensor
#### No addressing
In this mode, one sensor is connected to one pin of the MK; to work with it, it is not necessary to first read the address and write it to the program.
You can connect several sensors, specify your own pin for each, see the *one_pin_one_sensor* example.
```cpp
MicroDS18B20<pin1> sensor1;
MicroDS18B20<pin2> sensor2;
// ... etc
```

#### With addressing
In this mode, you can connect as many sensors as you like to one pin of the MK, but to work with them, you will need to enter unique sensor addresses into the program.
At the time of reading the address, only one sensor should be connected to the pin! An example is *address_read*.
For further work, the addresses are stored in arrays on the program side and transferred to the sensors during initialization, the pin is the same:
```cpp
uint8_t addr1[] = {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE};
uint8_t addr2[] = {0x28, 0xFF, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

MicroDS18B20<pin, addr1> sensor1;
MicroDS18B20<pin, addr2> sensor2;
// ... etc
```
Also, the address can be changed while the program is running, see the documentation above.

#### Read temperature
Reading the temperature is divided into two stages - requesting and receiving data. The request is made by the `requestTemp()` function. After receiving a request
the sensor starts measuring the temperature, which lasts from 90 to 750 ms depending on the configured accuracy *(by default, the accuracy is maximum,
conversion takes 750 ms)*. If you read the temperature before the end of the conversion, the sensor will return the result of the previous measurement,
so the examples use a 1 second delay or timer poll. You can get the temperature using `getTemp()` [float] or `getTempInt()` [int].
If the received data is corrupted or the sensor is missing from the linkII - the function will return the previous successfully read temperature value.
**Note:** on repeated calls `getTemp()` does not request a new temperature from the sensor (long function execution),
instead, it simply returns the previous result until a new `requestTemp()` request is made.

In version 3.5 of the library, it became possible to separately request the temperature and determine the correctness of the received data, so that only after that they can be read
and apply in the program - the `readTemp()` function. It also allows you to determine the connection status and whether everything is in order with the sensor.
To read the temperature, it is recommended to use a construction of the form:
```cpp
if (sensor.readTemp()) value = sensor.getTemp();
// else error handling
```
where `readTemp()` requests data from the sensor and returns `true` if it was read correctly. After that, you can get the current temperature from `getTemp()`,
which no longer requests the temperature from the sensor, but returns the result read in `readTemp()`.

#### We connect many sensors to one object
In version 3.9 of the library, it became possible to connect as many sensors as you like to one MicroDS18B20 object without creating an array of objects (as in older versions).
You need to create a two-dimensional array of addresses and transfer it to the library, also specifying the number of sensors on the line (the maximum is possible if it changes during the program).
This allows you to save some memory, but you can go further - put the array of sensor addresses in PROGMEM so that they do not hang in RAM.
Initialization in this case looks like this: `MicroDS18B20<pin, DS_ADDR_MODE, count>;` or `MicroDS18B20<pin, DS_ADDR_MODE, count, DS_PROGMEM>;` for PROGMEM mode.
Addresses are passed to `setAddress()`, and for polling, we simply pass the sensor index to the same functions as before. See the examples *async_read_many_bus*, *async_read_many_bus_pgm* and the documentation section *METHODS FOR SENSOR BUS*.

<a id="wiring"></a>
## Subkeycranberries
![scheme](/doc/scheme.png)
P.S. Instead of a 4.7k resistor, you can use two 10k in parallel =)

<a id="examples"></a>
See **examples** for other examples!
## One sensor without addressing
```cpp
// one sensor is better to read without addressing, it saves a lot of memory
#include <microDS18B20.h>
MicroDS18B20<2> sensor;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // temperature request
  sensor.requestTemp();
  
  // use a millis() timer instead of delay, async_read example
  delay(1000);
  
  // check if the read was successful and output
  if (sensor.readTemp()) Serial.println(sensor.getTemp());
  else Serial.println("error");
}
```

## Multiple sensors without addressing
```cpp
// 2 or more sensors are UNFAVORABLE to use in this mode! But you can

#include <microDS18B20.h>
// Sensors on D2 and D3
MicroDS18B20<2> sensor1;
MicroDS18B20<3> sensor2;

void setup() {
  Serial.begin(9600);
}

void loop() {
  // temperature request
  sensor1.requestTemp();
  sensor2.requestTemp();

  // use a millis() timer instead of delay, async_read example
  delay(1000);

  // FIRST SENSOR
  Serial.print("t1: ");
  
  // just display the temperature of the first sensor
  Serial.print(sensor1.getTemp());

  // SECOND SENSOR
  Serial.print(", t2: ");
  
  // check if the read was successful and output
  if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
  else Serial.println("error");
}
```

## Read address
```cpp
#include <microDS18B20.h>

// only one sensor is connected to the pin!
MicroDS18B20 <2> sensor; // Create a thermometer without an address on pin D2
uint8_t address[8]; // Create an array for the address

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the address of the thermometer into the specified array
  if (sensor.readAddress(address)) { // if successful, output
    Serial print('{');
    for (uint8_t i = 0; i < 8; i++) {
      Serial.print("0x");
      Serial.print(address[i], HEX); // Display address
      if (i < 7) Serial.print(", ");
    }
    Serial.println('}');

  }else Serial.println("Not connected");
  delay(1000);
}
```

## Several addressable sensors
```cpp
// 2 or more sensors are more profitable to use with addressing on one pin
#include <microDS18B20.h>
#define DS_PIN 2 // pin for thermometers

// Unique addresses of sensors - you can read in the address_read example
uint8_t s1_addr[] = {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE};
uint8_t s2_addr[] = {0x28, 0xFF, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

MicroDS18B20<DS_PIN, s1_addr> sensor1; // Create a thermometer with address
MicroDS18B20<DS_PIN, s2_addr> sensor2; // Create a thermometer with address

void setup() {
  Serial.begin(9600);
}

void loop() {
  // asynchronous reading of several sensors, see async_read_many example
  sensor1.requestTemp(); // Request temperature conversion
  sensor2.requestTemp();

  delay(1000); // expect result
  
  Serial.print("t1: ");
  if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
  else Serial.println("error");

  Serial.print("t2: ");
  if (sensor2.readTemp()) Serial.println(sensor2.getTemp());
  else Serial.println("error");
}
```

## Asynchronous polling of a bunch of sensors
```cpp
// an example of a compact asynchronous polling of sensors on a software timer
// https://alexgyver.ru/lessons/time/

// number of sensors for convenience
#define DS_SENSOR_AMOUNT 5

// create a two-dimensional array with addresses
uint8_t addr[][8] = {
  {0x28, 0xFF, 0x78, 0x5B, 0x50, 0x17, 0x4, 0xCF},
  {0x28, 0xFF, 0x99, 0x80, 0x50, 0x17, 0x4, 0x4D},
  {0x28, 0xFF, 0x53, 0xE5, 0x50, 0x17, 0x4, 0xC3},
  {0x28, 0xFF, 0x42, 0x5A, 0x51, 0x17, 0x4, 0xD2},
  {0x28, 0xFF, 0xCD, 0x59, 0x51, 0x17, 0x4, 0xFE},
};

#include <microDS18B20.h>
// specify DS_ADDR_MODE to connect the address block
// and create an array of sensors on pin D2
MicroDS18B20<2, DS_ADDR_MODE> sensor[DS_SENSOR_AMOUNT];

void setup() {
  Serial.begin(9600);
  // set addresses
  for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
    sensor[i].setAddress(addr[i]);
  }
}

void loop() {
  // construction of a software timer for 1c
  static uint32_t tmr;
  if (millis() - tmr >= 1000) {
    tmr = millis();

    // output readings to the port
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      Serial.print(sensor[i].getTemp());
      Serial print(',');
    }
    Serial.println();

    // request new
    for (int i = 0; i < DS_SENSOR_AMOUNT; i++) {
      sensor[i].requestTemp();
    }
  }
}
```

<a id="versions"></a>
## Versions
- v3.0 - Library moved to template! The old examples are NOT COMPATIBLE. Optimization, new tricks.
- v3.0.1 - example added
- v3.1 - added the ability to change the address on the fly
- v3.1.1 - microOneWire split into .h and .cpp
- v3.2 - fixed negative temperatures
- v3.3 - split into files
- v3.4 - added online check of the sensor and buffer, in case of a read error, the last read value is returned
- v3.5 - optimization, stability improvement, reading correctness, online() works with addressing, getTempInt() and readTemp() methods added, DS_TEMP_TYPE removed
- v3.6 - compilation error fixed, GyverCore support added (thanks to ArtemiyKolobov)
- v3.7 - fixed readTemp() error at 0 degrees
- v3.8 - small optimization. Compatible with ESP32
- v3.9 - added extended addressing mode and storage of addresses in PROGMEM
- v3.10 - optimization, increased stability

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!