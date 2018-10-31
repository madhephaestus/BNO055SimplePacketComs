/*
 * GetIMU.cpp
 *
 *  Created on: Jun 7, 2018
 *      Author: hephaestus
 */

#include "GetIMU.h"
#include <Wire.h>
//User function to be called when a packet comes in
// Buffer contains data from the packet cming in at the start of the function
// User data is written into the buffer to send it back
void GetIMU::event(float * buffer) {
	if(!started)
		return;
	for(int i=0;i<NUM_IMU_VALUES;i++){

		buffer[i]=bufferINTERNAL[i];
	}

}
void GetIMU::loop() {
	if(!started)
		return;
#if defined(ARDUINO_ARCH_ESP32)
	//Wire.reset();
	//bno->begin(Adafruit_BNO055::OPERATION_MODE_NDOF_FMC_OFF);
#endif
	imu::Vector<3>e = bno->getVector(Adafruit_BNO055::VECTOR_EULER);
	imu::Vector<3> a = bno->getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
	imu::Vector<3>v = bno->getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
	imu::Vector<3>g = bno->getVector(Adafruit_BNO055::VECTOR_GRAVITY);

	bufferINTERNAL[0] = a.x();
	bufferINTERNAL[1] = a.y();
	bufferINTERNAL[2] = a.z();
	bufferINTERNAL[3] = v.x();
	bufferINTERNAL[4] = v.y();
	bufferINTERNAL[5] = v.z();

	bufferINTERNAL[6] = g.x();
	bufferINTERNAL[7] = g.y();
	bufferINTERNAL[8] = g.z();

	bufferINTERNAL[9] = e.z();
	bufferINTERNAL[10] = e.y();
	bufferINTERNAL[11] = e.x();
}
void GetIMU::startSensor(Adafruit_BNO055 * _bno){
	//Wire.reset();
	//if(bno==NULL)
	bno = _bno;
	started=true;
	Serial.print(started);

}
void GetIMU::print(){
	if(!started)
		return;
	Serial.print("\r\n Acceleration= "
			+String(bufferINTERNAL[0])+" , "
			+String(bufferINTERNAL[1])+" , "
			+String(bufferINTERNAL[2])+"\r\n Gyro= "
			+String(bufferINTERNAL[3])+" , "
			+String(bufferINTERNAL[4])+" , "
			+String(bufferINTERNAL[5])+"\r\n Gravity= "
			+String(bufferINTERNAL[6])+" , "
			+String(bufferINTERNAL[7])+" , "
			+String(bufferINTERNAL[8])+"\r\n Euler= "
			+String(bufferINTERNAL[9])+" , "
			+String(bufferINTERNAL[10])+" , "
			+String(bufferINTERNAL[11])
	);
	/* Display calibration status for each sensor. */
	uint8_t system, gyro, accel, mag = 0;
	bno->getCalibration(&system, &gyro, &accel, &mag);
	Serial.print("\r\n CALIBRATION: Sys=");
	Serial.print(system, DEC);
	Serial.print(" Gyro=");
	Serial.print(gyro, DEC);
	Serial.print(" Accel=");
	Serial.print(accel, DEC);
	Serial.print(" Mag=");
	Serial.println(mag, DEC);
}
