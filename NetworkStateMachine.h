#ifndef NetworkStateMachine_h
#define NetworkStateMachine_h
#include "Event.h"
#include "StateMachine.h"
#include "Task.h"
#include "common.h"
#include <ESP8266WiFi.h>
class Event;

extern status_type g_status;

extern Event WLANConnected;
extern Event WLANDisconnected;
extern Event ServerConnected;
extern Event ServerDisconnected;
extern Event ClockSynced;

extern StateMachine NetSM;

extern Task WifiConnectionTask;
extern Task ServerConnectionTask;
extern Task ClockTickTask;
extern Task ClockSyncTask;

void wifi_connection();
void network_sm_trans (Event *event_, int *current_state);
void network_sm_run(int *current_state);

//server functions
void server_connection();
data_type send_frame(data_type frame);

void clock_sync();
#define HOTSPOT_NAME "ElecMonitor"

//eeprom functions
void save_monitor_id(int uid);
int get_monitor_id();
#define EEPROM_SIZE 32
#endif
