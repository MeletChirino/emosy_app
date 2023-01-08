#ifndef common_h
#define common_h
#include "RyemTime.h"

#define NUM_SENSORS 24
#define MINS(min) (int)(min * 60 * 1000)
#define HOURS(hour) (int)(hour * 3600 * 1000)

typedef struct{
	bool WLAN;
	bool server;
	unsigned int stored_data;
	time_type last_sync;
	time_type time;
	unsigned int port;
	int uid;
	//string token;
	//string host;
	//string user;
} status_type;

typedef unsigned int data_type;

#define ERROR_FRAME 0x00000000
#define SERVER_CONNECTION_MASK 0x19950000
#define SERVER_CONNECTION_OK_FRAME 0x19730905
#define UNREGISTERED_MONITOR_FRAME 0x19960314
#define BUSY_SERVER_FRAME 0xF0AB0ACF

#define CLOCK_SYNC_REQ_MASK 0x19750000
#define CLOCK_SYNC_REQ_OK 0x19751116
#define CLOCK_SYNC_MASK 0x19730000
#define TIME_TRANSMISSION_OK 0x20151017 // Martina

#define DATA_TRANSMISSION_REQ_OK 0x20070621 //Luis Esteban
#define DATA_TRANSMISSION_OK 0x20070621 //Luis Esteban
#define DATA_TRANSMISSION_REQ_MASK 0x20070000
#define DATA_TRANSMISSION_MASK 0x20070000
// clock sync macros
#define HOST "192.168.0.35"
#define PORT 52520
#define CHECK_CLOCK_SYNC(frame) (frame >> 30) & 0b11
#define CHECK_CLOCK_SYNC_VAL 1

#endif
