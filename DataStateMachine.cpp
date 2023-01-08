#include <vector>
#include "common.h"
#include "Event.h"
#include "Task.h"
#include <Arduino.h>
#include "DataStateMachine.h"

#define DEBUG 0

// array that saves data
std::vector<save_data_type> stored_data(2000);
uint8_t mux_pin[8] = {
  1, 2, 3, 4, 5, 6, 7, 8
};

void data_sm_trans(Event *event_, int *current_state) {
  Serial.print("DATA:\n");
  switch (DataSM.current_state) {
    case 0: // 
      if (event_ == &ClockSynced){ 
        DataSM.current_state = 1;
	SendDataTask.enable();
      }
      break;
    case 1: // to save data locally 
      if (event_ == &ServerDisconnected){ 
        DataSM.current_state = 2;
	SaveDataTask.enable();
      }
      break;
    case 2: // to connected state 
      if (event_ == &ServerConnected) {
        DataSM.current_state = 1;
	send_stored_data();
      }
      break;
  }
}

void data_sm_run(int *current_state) {
  Serial.print("DATA:\n");
  switch (DataSM.current_state) {
    case 0: // stand by state
      Serial.println("stand by state");
	  // do nothing until server is connected
	  break;
    case 1: // Send Data to cloud
      Serial.println("Data send state");
      SendDataTask.run();
      break;
    case 2: // Store data locally
      Serial.println("Save data state");
      SaveDataTask.run();
      break;
  }
}
void send_live_data(){
  Serial.println("Send data live");
  data_type data = get_data();
  if (send_data(data, g_status.time))
    // if data sent is succesfull it can go back
    return;
  else 
    save_data(data, g_status.time);
}

void save_live_data(){
  save_data(get_data(), g_status.time);
}

bool send_data(data_type data, time_type current_time){
  Serial.println("Data Send");
  data_type response = send_frame(
      DATA_TRANSMISSION_REQ_MASK | g_status.uid
      );
  if (response == DATA_TRANSMISSION_REQ_OK){
    Serial.println("Data transmission request ok");
    response = send_frame(data);
    if (response == DATA_TRANSMISSION_OK){
      Serial.println("Data transmitted ok");
      response = send_frame(current_time | 0x10000000);//send time value
      if(response == TIME_TRANSMISSION_OK){
	Serial.println("time transmitted ok");
	//everything happened as expected
	return true;
      } else if (response == ERROR_FRAME){
	ServerDisconnected.happen();
      }
    }

  } else if (response == ERROR_FRAME){
    //do something to trace errors
    ServerDisconnected.happen();
  }
  return false;
}

inline void save_data(data_type data, time_type current_time){
  stored_data[g_status.stored_data].data = data;
  stored_data[g_status.stored_data].time = current_time;
  g_status.stored_data++;
  Serial.print("Saving data [");
  Serial.print(g_status.stored_data);
  Serial.print("] data\n\n");
}

void send_stored_data(){
  Serial.println("Sending stored data");
  for(int i = g_status.stored_data - 1; i >= 0; i--){
    Serial.print("sending [");
    Serial.print(i);
    Serial.print("](");
    Serial.print(stored_data[i].data);
    Serial.print("),(");
    Serial.print(stored_data[i].time);
    Serial.print(");\n");
    if(send_data(
	  stored_data[i].data,
	  stored_data[i].time)
      )
      g_status.stored_data--;
    else
      return;
  }
}

data_type get_data(){
  // consigue todas las lecturas de los sensores y guardala en un int de 32bits
  data_type data = DATA_TRANSMISSION_MASK;
  for(int i = 0; i <= NUM_SENSORS; i++){
    set_mux_val(i);
    //data |= digitalRead(READ_PIN);
    delay(10);

    //borrar antes de prod
    data |= GET_PIN(i);
  }
  Serial.print("Get data = ");
  Serial.println(data);
  return data;
}

inline void set_mux_val(int val){
  //Serial.println("Set Mux Val");
  for (int i = 0; i < MUX_SIZE; i++){
    digitalWrite(mux_pin[i], (val >> i) & 0b1);
  }
}
