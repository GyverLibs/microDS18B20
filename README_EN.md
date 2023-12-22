This is an automatic translation, may be incorrect in some places. See sources and examples!

# Microds18b20
Light library for working with 1-Wire (Onewire) thermometers Dallas DS18B20
- asynchronous work (without expectation of conversion)
- Work with several sensors on one pin (addressing mode)
- storage of an array of addresses in ProGMEM memory
- Work with one sensor on pin (without using addressing)
- calculation of temperature in integers and with a floating point
- reading raw data for cases of strong memory savings
- checking the correctness of the resulting temperature
- customized resolution of transformation
- Data authenticity check
- checking the correctness of the sensor

## compatibility
Compatible with all arduino platforms (used arduino functions)
- does not work on fresh versions of SDK ESP32
- It does not work on Digispark, some kind of conflict with a compiler.Use the nucleus [Attinycore Universal] (https://github.com/spencekonde/atTinycore) with the setting of Attiny85 + Micronucleous (Digispark) instead

## Content
- [installation] (# Install)
- [initialization] (#init)
- [use] (#usage)
- [connection] (#wiring)
- [Examples] (# ExamPles)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- The library can be found by the name ** Microds18b20 ** and installed through the library manager in:
    - Arduino ide
    - Arduino ide v2
    - Platformio
- [download the library] (https://github.com/gyverlibs/microds18b20/archive/refs/heads/main.zip). Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="init"> </a>
## initialization
`` `CPP
// One sensor on pin without addressing
Microds18b20 <uint8_t pin> ds;

// Several sensors on the pin with addressing, indicate the address (address - array Uint8_T)
Microds18b20 <uint8_t pin, uint8_t *address> ds;

// We indicate that we will work with addressing.We will transmit the address itself later (in Setaddress ())
Microds18b20 <uint8_t pin, ds_addr_mode>;

// indicate that we will work with addressing, and there will be several (amount) sensors on the line
// See example async_read_many_bus
Microds18b20 <uint8_t pin, ds_addr_mode, uint8_t amount>;

// We indicate that we will work with addressing, there will be several sensors on the line, and we will store the addresses in Progmem
// see pr.Imer async_read_many_bus_pgm
Microds18b20 <uint8_t pin, ds_addr_mode, uint8_t amount, ds_progmem>;
`` `

<a id="usage"> </a>
## Usage
`` `CPP
// ============= Methods of class ========================
Bool Readaddress (Uint8_t *Addressarry);// Read the unique address of the thermometer in the array.[True, if successful]
VOID Setaddress (Uint8_T *Addr);// Set (change) address
VOID Setresolution (Uint8_T Resolution);// Set a resolution 9-12 bits
Bool online ();// Check the connection with the sensor (True - online sensor).The tire should be fitted

VOID RequestTemp ();// Request a new temperature transformation
Bool Readtemp ();// Read the temperature from the sensor.[true if successfully]

Float gettemp ();// get the temperature value in Float
int16_t gettempint ();// get the temperature value in int
Int16_T GETRAW ();// get a "raw" temperature value (16 times more than real temperature)

// ======= Methods for the tire of sensors =========
// See examples async_read_many_bus and async_read_many_bus_pgm
VOID Setresolutionall (Uint8_T Res);// Set a resolution of 9-12 bits for all sensors on the line
VOID Setresolution (Uint8_T Resolution, Uint8_T IDX);// Set a resolution 9-12 bits (sensor under the IDX number)
Bool Online (Uint8_T IDX);// Check the connection (sensor under the IDX number)

VOID RequestTempall ();// Temperature request for all sensors on the line
VOID REQUESTEMP (Uint8_T IDX);// Request a new temperature conversion (sensor under the IDX number)
Bool Readtemp (Uint8_T IDX);// Read the temperature from the sensor (sensor under the IDX number)

Float Gettemp (Uint8_T IDX);// get the temperature value in Float (sensor under the IDX number)
Int16_T Gettempint (Uint8_T IDX);// get the temperature value in int (sensor under the IDX number)
Int16_T Getraw (Uint8_T IDX);// Get a "raw" temperature value (sensor under the IDX number)

// =========== Functions outside the class ============
int DS_RAWTOINT (inta);// Transform RAW data into temperature int
Float DS_RAWTOFLOAT (Inta);// Transform RAW data to Float temperature

// ========ward
// write down before connecting the library
#define ds_check_crc [True / False] // Checking of data authenticity.When disconnecting, it will give out incorrect value when gear failure (def. True)
#define ds_crc_use_table [True / False] // Use a table for CRC.Faster, but +256 bytes Flash (<1mx vs ~ 6mx) (silence. False)

// ====ward
// Time of transformation from accuracy
accuracy |time
12 bits |750 ms
11 bit |375 ms
10 bits |187 ms
9 bits |93 ms
`` `

### Work with a sensor
#### without addressing
In this mode, one sensor is connected to one PIN MK, it does not require a preliminary reading of the address and write it to the program to work with it.
You can connect several sensors, indicate each PIN, see example *One_pin_one_Sensor *.
`` `CPP
Microds18b20 <PIN1> Sensor1;
Microds18b20 <PIN2> Sensor2;
// ... and so on
`` `

### with addressing
In this mode, you can connect as many sensors to one PIN MK, but to work with them you will need to bring unique sensors addresses into the program.
At the time of reading the address to Pin, only one sensor must be connected!Example - *Address_read *.
For further work, the addresses are stored in arrays on the side of the program and transmitted to the sensors during initialization, the PIN is indicated the same:
`` `CPP
uint8_t addr1 [{0x28, 0xff, 0xcd, 0x59, 0x51, 0x17, 0x4, 0xfe};
uint8_t addr2 [] = {0x28, 0xff, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

Microds18b20 <Pin, Addr1> Sensor1;
Microds18b20 <Pin, Addr2> SensoR2;
// ... and so on
`` `
Also, the address can be changed during the program, see the documentation above.

#### Reading Temperature
Reading the temperature is divided into two stages - a request and obtaining data.The request is made by the `REQUESTEMP ()` function.After receiving the request
The sensor begins a temperature measurement, which lasts from 90 to 750 ms, depending on the configured accuracy *(by default, the accuracy is maximum,
The transformation lasts 750 ms)*.If you read the temperature before the end of the transformation, the sensor will return the result of the previous measurement,
Therefore, the examples use a delay or a survey by a timer for 1 second.You can get the temperature using `gettemp ()` [float] or `gettempint ()` [int].
If the accepted data is damaged or the sensor is absent on the line, the function will return the previous successfully read temperature value.
** Note: ** During repeated calls, `gettemp ()` does not request a new temperature from the sensor (long execution of the function),
Instead, she simply returns the previous result until a new request is made `RequestTemp ()`.

In the version of Library 3.5, it became possible to separately request the temperature and determine the correctness of the data obtained, so that only after that read them
And apply in the program - the function `Readtemp ()`.This also allows you to determine the condition of the connection and whether everything is in order with the sensor.
To read temperature, it is recommended to use the design of the type:
`` `CPP
if (Sensor.Readtemp ()) value = sensor.gettemp ();
// Else error development
`` `
Where `Readtemp ()` Causes the data from the sensor and returns `true` if they are read correctly.After that, you can pick up the current temperature from `gettemp ()`,
Which no longer requests the temperature from the sensor, but gives the result read in `readtemp ()` result.

### We connect a lot of sensors to one object
In the version of Library 3.9, it became possible to connect as many sensors to one object Microds18b20, without creating an array of objects (as in old versions).
It is necessary to create a two -dimensional array of addresses and transfer it to the library, also indicating the number of sensors on the line (you can maximum if it changes in the process of the program).
This allows you to save a little memory, but you can go further - put an array of sensors in the Progmem so that they do not hang in RAM.
In this case, the initialization looks like this: `Microds18b20 <Pin, ds_addr_mode, quantity>;` or `Microds18b20 <pin, ds_addr_mode, Kolich-in, DS_PROGMEM>;` for Progmem mode.
The addresses are transmitted to `setaddress ()`, and for the survey we simply transmit the sensor index to the same functions as before.See examples *async_read_many_bus *, *async_read_Many_bus_pgm *and documentation section *Methods for the sensors tire *.

<a id="wiring"> </a>
## connection
! [Scheme] (/doc/scheme.png)
P.S.Instead of a resistor at 4.7k, you can use two in parallel by 10k =)

<a id="EXAMPLASX> </A>
The rest of the examples look at ** Examples **!
## one sensor without addressing
`` `CPP
// One sensor is better to read without addressing, this strongly saves memory
#include <micrrods18b20.h>
Microds18b20 <2> Sensor;

VOID setup () {
  Serial.Begin (9600);
}

VOID loop () {
  // Temperature request
  Sensor.requestTemp ();
  
  // Instead of DELAY, use a timer on Millis (), an example async_read
  DELAY (1000);
  
  // Check the success of reading and display
  if (Sensor.Readtemp ()) serial.println (Sensor.gettemp ());
  Else serial.println ("error");
}
`` `

## several sensors without addressing
`` `CPP
// 2 and more sensors are unprofitable to use in this mode!But you can

#include <micrrods18b20.h>
// sensors for d2 and d3
Microds18b20 <2> Sensor1;
Microds18b20 <3> Sensor2;

VOID setup () {
  Serial.Begin (9600);
}

VOID loop () {
  // Temperature request
  Sensor1.requesttemp ();
  Sensor2.requesttemp ();

  // Instead of DELAY, use a timer on Millis (), an example async_read
  DELAY (1000);

  // The first sensor
  Serial.print ("t1:");
  
  // just display the temperature of the first sensor
  Serial.print (Sensor1.gettemp ());

  // Second sensationK.
  Serial.print (", t2:");
  
  // Check the success of reading and display
  if (Sensor2.Readtemp ()) serial.println (Sensor2.gettemp ());
  Else serial.println ("error");
}
`` `

## Reading the address
`` `CPP
#include <micrrods18b20.h>

// only one sensor is connected to the pin!
Microds18b20 <2> Sensor;// Create a thermometer without address on pin D2
uint8_t address [8];// Create an array for the address

VOID setup () {
  Serial.Begin (9600);
}

VOID loop () {
  // read the thermometer address in the specified array
  if (Sensor.Readaddress (Address)) {// If successfully, we display
    Serial.print ('{');
    for (uint8_t i = 0; i <8; i ++) {
      Serial.print ("0x");
      Serial.print (address [i], hex);// display the address
      if (i <7) serial.print (",");
    }
    Serial.println ('}');

  } Else serial.println ("Not Connected");
  DELAY (1000);
}
`` `

## a few sensors with addressing
`` `CPP
// 2 or more sensors is more profitable to use with addressing on one pin
#include <micrrods18b20.h>
#define ds_pin 2 // PIN for thermometers

// Unique sensors - you can count in the example of Address_read
uint8_t s1_addr [] = {0x28, 0xff, 0xcd, 0x59, 0x51, 0x17, 0x4, 0xfe};
uint8_t s2_addr [] = {0x28, 0xff, 0x36, 0x94, 0x65, 0x15, 0x2, 0x80};

Microds18b20 <ds_pin, s1_addr> sensor1;// Create a thermometer with addressing
Microds18b20 <ds_pin, s2_addr> sensor2;// Create a thermometer with addressing

VOID setup () {
  Serial.Begin (9600);
}

VOID loop () {
  // Asynchronous reading of several sensors see async_read_many
  Sensor1.requesttemp ();// Request temperature transformation
  Sensor2.requesttemp ();

  DELAY (1000);// We expect the result
  
  Serial.print ("t1:");
  if (Sensor1.Redtemp ()) serial.println (Sensor1.gettemp ());
  Else serial.println ("error");

  Serial.print ("t2:");
  if (Sensor2.Readtemp ()) serial.println (Sensor2.gettemp ());
  Else serial.println ("error");
}
`` `

## asynchronous survey packs of sensors
`` `CPP
// An example of a compact asynchronous survey of sensors on a software timer
// https://alexgyver.ru/lessons/time/

// number of sensors for convenience
#define ds_sensor_amount 5

// Create a two -dimensional array with addresses
uint8_t addr [] [8] = {{
  {0x28, 0xff, 0x78, 0x5b, 0x50, 0x17, 0x4, 0xcf},
  {0x28, 0xff, 0x99, 0x80, 0x50, 0x17, 0x4, 0x4d},
  {0x28, 0xff, 0x53, 0xe5, 0x50, 0x17, 0x4, 0xc3},
  {0x28, 0xff, 0x42, 0x5a, 0x51, 0x17, 0x4, 0xd2},
  {0x28, 0xff, 0xcd, 0x59, 0x51, 0x17, 0x4, 0xfe},
};

#include <micrrods18b20.h>
// specify ds_addr_mode to connect an addressing unit
// and create an array of sensors on pin D2
Microds18b20 <2, ds_addr_mode> sensor [ds_sensor_amount];

VOID setup () {
  Serial.Begin (9600);
  // Install the addresses
  for (int i = 0; i <ds_sensor_amount; i ++) {
    Sensor [i] .Setaddress (Addr [i]);
  }
}

VOID loop () {
  // The design of the program timer for 1C
  Static uint32_t tmr;
  if (millis () - tmr> = 1000) {
    TMR = Millis ();

    // We display the readings to the port
    for (int i = 0; i <ds_sensor_amount; i ++) {
      Serial.print (Sensor [i] .gettemp ());
      Serial.print (',');
    }
    Serial.println ();

    // Request new ones
    for (int i = 0; i <ds_sensor_amount; i ++) {
      Sensor [i] .requesttemp ();
    }
  }
}
`` `

<a id="versions"> </a>
## versions
- V3.0 - The library moved to the template!Old examples are incompatible.Optimization, new tricks.
- v3.0.1 - Added example
- v3.1 - added the possibility of changing the address on the fly
- V3.1.1 - Microonewire is divided into .h and .cpp
- v3.2 - negative temperatures are fixed
- v3.3 - divided into files
- v3.4 - added the sensor online and buffer, with an error of reading, the last value read is returned
- V3.5 - optimization, increasing stability, checking the correctness of reading, online () works with addressing, added Gettempint () and Readtemp (), abolished ds_temp_type
- V3.6 - Fixed compilation error, added support Gyvercore (thanks Artemiykolobov)
- v3.7 - ReadTemp () error is fixed at 0 degrees
- V3.8 - slight optimization.Compatibility with ESP32
- v3.9 - added an extended addressing mode and storage of addresses in Progmem
- v3.10 - optimization, stability increased

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code