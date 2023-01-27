#include "common.h"
#include "WiFiManager.h" // https://github.com/tzapu/WiFiManager
#include "Task.h"
#include "StateMachine.h"
#include "Event.h"
#include "NetworkStateMachine.h"
#include "RyemTime.h"
#include "DataStateMachine.h"
#include "RyemMux.h"

void clock_tick();
void show_time();

// --------------- Events ---------------
Event WLANConnected;
Event WLANDisconnected;
Event ServerConnected;
Event ServerDisconnected;
Event ClockSynced;

// --------------- State Machines ---------------
StateMachine NetSM(
  network_sm_trans,
  network_sm_run
);

StateMachine DataSM(
  data_sm_trans,
  data_sm_run
);

// --------------- Tasks ---------------

Task SendDataTask(MINS(5), send_live_data);
Task SaveDataTask(MINS(5), save_live_data);
Task WifiConnectionTask(MINS(3), wifi_connection, true);
Task ServerConnectionTask(MINS(1), server_connection, true);
Task ClockSyncTask(MINS(1), clock_sync, true);
Task ClockTickTask(1000, clock_tick);

// --------------- Mux ---------------
RyemMux EmosyMux({1, 2, 3, 4, 5}, 6);

status_type g_status;

auto serial_instance = Serial;

void setup() {
  Serial.begin(115200);
  g_status.stored_data = 0;
  g_status.uid = get_monitor_id();

  // Mux init
  EmosyMux.set_input_n(24);
  EmosyMux.init();

  // Event init
  WLANConnected.init(1);
  WLANDisconnected.init(2);
  ServerConnected.init(3);
  ServerDisconnected.init(4);
  ClockSynced.init(5);

  // Data State Machine init
  DataSM.init(22);
  DataSM.attach_event(&ServerConnected);
  DataSM.attach_event(&ServerDisconnected);
  DataSM.attach_event(&ClockSynced);

  // Network State Machine init
  NetSM.init(31);
  NetSM.attach_event(&WLANConnected);
  NetSM.attach_event(&WLANDisconnected);
  NetSM.attach_event(&ServerConnected);
  NetSM.attach_event(&ServerDisconnected);
  wifi_connection();

}

void loop() {
  NetSM.run();
  DataSM.run();
  ClockTickTask.run();
}

void show_time(){
  Serial.print("year "); Serial.println(GET_YEAR(g_status.time));
  Serial.print("month "); Serial.println(GET_MONTH(g_status.time));
  Serial.print("yday "); Serial.println(GET_DAY(g_status.time));
  Serial.print("hour "); Serial.println(GET_HOUR(g_status.time));
  Serial.print("min "); Serial.println(GET_MIN(g_status.time));
  Serial.print("sec "); Serial.println(GET_SECONDS(g_status.time));
}

void clock_tick(){
  Serial.print("UPDATE TIME -> ");
  Serial.println(g_status.time);
  g_status.time++;
  //show_time();
}
