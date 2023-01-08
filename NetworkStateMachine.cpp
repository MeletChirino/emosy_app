#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Arduino.h>
#include "NetworkStateMachine.h"
#include "Event.h"
#include "RyemTime.h"
#include <EEPROM.h>//https://github.com/esp8266/Arduino/blob/master/libraries/EEPROM/EEPROM.h
#define DEBUG 0

void network_sm_trans(Event *event_, int *current_state) {
  Serial.print("NET:\n");
  switch (NetSM.current_state) {
    case 0: // WiFi Connection
      if (event_ == &WLANConnected) {
        NetSM.current_state = 1;
        delay(100);
      }
      break;
    case 1: // Server Connection
      Serial.print("Server connection\n");
      if (event_ == &ServerConnected) {
        NetSM.current_state = 2;
        delay(100);
      }
      if (event_ == &WLANDisconnected){
        NetSM.current_state = 0;
        delay(100);
      }
      break;
    case 2: // Operative state
      if (event_ == &WLANDisconnected){
        NetSM.current_state = 0;
        delay(100);
      }
      if (event_ == &ServerDisconnected) {
        NetSM.current_state = 1;
        delay(100);
      }
      break;
  }
      Serial.print("leaving transition methods\n");
}

void network_sm_run(int *current_state) {
  Serial.print("NET:\n");
  switch (NetSM.current_state) {
    case 0: // WiFi connection state
      Serial.println("Wifi Connection state");
      WifiConnectionTask.run();
      break;
    case 1: // Server connection state
      Serial.println("Now You gotta connect to the server");
      ServerConnectionTask.run();
      break;
    case 2: // Operative state
      Serial.println("Operative state");
      ClockSyncTask.run();
      break;
  }
}

void wifi_connection() {
  Serial.print("Current ID");
  Serial.println(g_status.uid);
  WiFi.mode(WIFI_STA);

  WiFiManager wm;
  // Supress Debug information
  //wm.setDebugOutput(false);

  // Remove any previous network settings
  //wm.resetSettings();

  // set configportal timeout
  wm.setConfigPortalTimeout(120); // two minutes wait
  wm.setConnectTimeout(20);

  WiFiManagerParameter id_box("mon_id", "MonitorId", "1234", 50);

  if (g_status.uid >= 0xFFFF || g_status.uid == -1) {
    // If EEPROM space is filled with random stuff 
    // Wifi manager will ask for an ID
    Serial.println("Id not set");
    wm.addParameter(&id_box);
    wm.resetSettings();
  }

  if (!wm.autoConnect(HOTSPOT_NAME)) {
    Serial.println("failed to connect and hit timeout");
    delay(300);
    //reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(500);
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
    if (g_status.uid >= 0xFFFF || g_status.uid == -1){
      save_monitor_id(atoi(id_box.getValue()));
      ESP.restart();
    }

    //Save credentials
    WLANConnected.happen();
    Serial.println("going back");
  }
}

void server_connection() {
  Serial.println("Trying to connect the server");
  data_type response = send_frame(
      SERVER_CONNECTION_MASK | g_status.uid
      );
  Serial.print("\nfinal response = "); Serial.print(response); 
  if(response == SERVER_CONNECTION_OK_FRAME){
    ServerConnected.happen();
    delay(600);
  }
  else if (response == UNREGISTERED_MONITOR_FRAME){
    // if monitor is not registered ID is reset
    save_monitor_id(0xFFFFFFFF);
    ServerDisconnected.happen();
    WLANDisconnected.happen();
  }
  else if (response == ERROR_FRAME){
    WLANDisconnected.happen();
  }
  Serial.println("leaving sever_connection()");
}

void clock_sync(){
  Serial.println("Syncing clock... ");
  data_type response = send_frame(
      CLOCK_SYNC_REQ_MASK | g_status.uid
      );
  Serial.print("\nfinal response = "); Serial.println(response);
  if (response == CLOCK_SYNC_REQ_OK){
    Serial.println("Clock sync requested");
    response = send_frame(CLOCK_SYNC_MASK | g_status.uid);
    if ((response & 0x40000000) == 0x40000000){
    Serial.println("sync to global status");
      g_status.time = (time_type)response;
      ClockTickTask.enable();
      g_status.last_sync = g_status.time;
      ClockSynced.happen();
    }
  } else if (response == ERROR_FRAME){
    ServerDisconnected.happen();
  }
  Serial.print("\ng_status = "); Serial.println(g_status.time);
}

data_type send_frame(data_type frame){
  // this function is going to repeat if response
  // indicates that server is busy handling other
  // requests
  data_type response = 0;
  WiFiClient client;
  do {
    if (!client.connect(HOST, PORT)) {
	  Serial.println("connection failed");
	  delay(500);
	  return ERROR_FRAME;
    }
    // This will send a string to the server
    Serial.println("sending data to server");
    Serial.print("Sending -> "); Serial.println(frame);
    if (client.connected()) {
	  for (int i = 0; i < 4; i++) {
	    client.write((frame >> i * 8) & 0xFF);
	  }
    }

    // wait for data to be available
    unsigned long timeout = millis();
    while (client.available() == 0) {
	  if (millis() - timeout > 5000) {
	    Serial.println(">>> Client Timeout !");
	    client.stop();
	    delay(600);
	    return ERROR_FRAME;
	  }
    }
    uint8_t i = 0;
    while (client.available()) {
	  unsigned int ch = (client.read());
	  response = (ch << i*8) | response;
	  i++;
    }
    client.stop();
    Serial.print("resp = 0x");
    Serial.println(response, HEX);
    if (response == BUSY_SERVER_FRAME)
      delay(5000);
  } while (response == BUSY_SERVER_FRAME);
  return response;
}

void save_monitor_id(int uid){
  Serial.println("Setting ID value");
  Serial.println(uid);
  EEPROM.begin(EEPROM_SIZE);
  //Write data into eeprom
  EEPROM.put(0, uid);
  EEPROM.commit();
  EEPROM.end();
}

int get_monitor_id(){
  EEPROM.begin(EEPROM_SIZE);
  //Read data from eeprom
  int readId;
  EEPROM.get(0, readId);
  Serial.print("Read Id = ");
  Serial.println(readId);
  EEPROM.end();
  return readId;
}
