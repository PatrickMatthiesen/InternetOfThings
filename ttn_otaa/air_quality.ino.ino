/* example1_basic.ino

 This example shows basic data retrieval from the SparkFun Indoor Air Quality Sensor - ENS160.

 Written by: 
	Elias Santistevan @ SparkFun Electronics October, 2022

 Product: 
  https://www.sparkfun.com/products/20844
 
 Repository:
	https://github.com/sparkfun/SparkFun_Indoor_Air_Quality_Sensor-ENS160_Arduino_Library

 SparkFun code, firmware, and software is released under the MIT
 License(http://opensource.org/licenses/MIT).

*/
#include <Wire.h>
#include "SparkFun_ENS160.h"

#include "SparkFunBME280.h"


SparkFun_ENS160 myENS; 
BME280 mySensor;

int ensStatus; 

void setup_air()
{
	Wire.begin();

	if( !myENS.begin() )
	{
		Serial.println("Could not communicate with the ENS160, check wiring.");
		while(1);
	}

  Serial.println("Example 1 Basic Example.");

	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	// Device needs to be set to idle to apply any settings.
	// myENS.setOperatingMode(SFE_ENS160_IDLE);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

	// There are four values here: 
	// 0 - Operating ok: Standard Operation
	// 1 - Warm-up: occurs for 3 minutes after power-on.
	// 2 - Initial Start-up: Occurs for the first hour of operation.
  //												and only once in sensor's lifetime.
	// 3 - No Valid Output
	ensStatus = myENS.getFlags();
	Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
	Serial.println(ensStatus);
	

  if (mySensor.beginI2C() == false) //Begin communication over I2C
  {
    Serial.println("The sensor did not respond. Please check wiring.");
    while(1); //Freeze
  }
}

uint8_t getCO2() {
  return myENS.getTVOC();
}

byte* prepareSendData(float humidity, float pressure, float altitude, float temperature, uint16_t tvoc, uint16_t eco2) {
    byte senddata[] = {
        (uint8_t)(int(humidity)),
        (uint8_t)(highByte(int(pressure / 10))),
        (uint8_t)(lowByte(int(pressure / 10))),
        (uint8_t)(highByte(int(altitude))),
        (uint8_t)(lowByte(int(altitude))),
        (uint8_t)((int(temperature) + 40) * 2),
        (uint8_t)(highByte(tvoc)),
        (uint8_t)(lowByte(tvoc)),
        (uint8_t)(highByte(eco2)),
        (uint8_t)(lowByte(eco2)),
        (uint8_t)(0) // Reserved byte
    };

    return senddata;
}


byte* getSensorData() {
    float humidity = mySensor.readFloatHumidity();
    float pressure = mySensor.readFloatPressure();
    float altitude = mySensor.readFloatAltitudeMeters();
    float temperature = mySensor.readTempC();
    uint16_t tvoc = myENS.getTVOC();
    uint16_t eco2 = myENS.getECO2();

    byte* senddata = prepareSendData(humidity, pressure, altitude, temperature, tvoc, eco2);

    return senddata;
    // Now you can use senddata for transmission
    // LMIC_setTxData2(1, senddata, sizeof(senddata), 0);
    // Serial.println("Data packet queued for transmission.");
}

void air_check()
{
	if( myENS.checkDataStatus() )
	{
		Serial.print("Air Quality Index (1-5) : ");
		Serial.println(myENS.getAQI());

		Serial.print("Total Volatile Organic Compounds: ");
		Serial.print(myENS.getTVOC());
		Serial.println("ppb");

		Serial.print("CO2 concentration: ");
		Serial.print(myENS.getECO2());
		Serial.println("ppm");

	  Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
    Serial.println(myENS.getFlags());

		Serial.println();
	}

	delay(2000);
}
