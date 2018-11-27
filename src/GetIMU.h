/*
 * GetIMU.h
 *
 *  Created on: Jun 7, 2018
 *      Author: hephaestus
 */

#ifndef LIBRARIES_MEDIUMKATFIRMWARE_SRC_GETIMU_H_
#define LIBRARIES_MEDIUMKATFIRMWARE_SRC_GETIMU_H_


#include <SimplePacketComs.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define NUM_IMU_VALUES 12
class GetIMU: public PacketEventAbstract {
private:
	Adafruit_BNO055 * bno;
//	imu::Vector<3> v;
//	imu::Vector<3> a;
//	imu::Vector<3> g;
//	imu::Vector<3> e;
	volatile float  bufferINTERNAL[NUM_IMU_VALUES];
	bool started;
	int updateIndex=0;
public:
	// Packet ID needs to be set
	GetIMU() :
		PacketEventAbstract(1804)	// Address of this event
{
		started=false;
		bno=NULL;
}
	//User function to be called when a packet comes in
	// Buffer contains data from the packet cming in at the start of the function
	// User data is written into the buffer to send it back
	void event(float * buffer);
	void loop();
	void startSensor(Adafruit_BNO055 * _bno);
	void print();
};


#endif /* LIBRARIES_MEDIUMKATFIRMWARE_SRC_GETIMU_H_ */
