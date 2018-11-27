#include <SimplePacketComs.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <BNO055SimplePacketComs.h>
#if defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <Preferences.h>
#include <Esp32SimplePacketComs.h>
#include <wifi/WifiManager.h>
#include <server/NameCheckerServer.h>
UDPSimplePacket coms;
WifiManager manager;
#else
#include <Teensy32SimplePacketComs.h>
HIDSimplePacket coms;
#endif

GetIMU * sensor;
long lastPrint = 0;
// Change this to set your team name
String * name = new String("IMU-Team21");
Adafruit_BNO055 bno;

void setup() {
	Serial.begin(115200);
#if defined(ARDUINO_ARCH_ESP32)
	manager.setup();
	coms.attach((PacketEventAbstract *) new NameCheckerServer(name));
#endif
	sensor = new GetIMU();
	/* Initialise the sensor */
	if (!bno.begin()) {
		/* There was a problem detecting the BNO055 ... check your connections */
		Serial.print(
				"Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
		while (1);
	}

	delay(1000);
	bno.setExtCrystalUse(true);
	/* Display the current temperature */
	int8_t temp = bno.getTemp();
	Serial.print("Current Temperature: ");
	Serial.print(temp);
	Serial.println(" C");
	Serial.println("");
	sensor->startSensor(&bno);
	coms.attach((PacketEventAbstract *) sensor);
}

void loop() {
#if defined(ARDUINO_ARCH_ESP32)
	manager.loop();
	if (manager.getState() != Connected)
		return;
#endif
	coms.server();
	sensor->loop();
	if (millis() - lastPrint > 20) {
		lastPrint = millis();

		sensor->print();
	}
}

