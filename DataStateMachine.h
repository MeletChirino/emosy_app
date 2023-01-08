#ifndef DataStateMachine_h
#define DataStateMachine_h
#include "Event.h"
#include "StateMachine.h"
#include "Task.h"
#include "RyemTime.h"
#include "common.h"
#include <vector>
class Event;

extern status_type g_status;

extern Event ServerConnected;
extern Event ServerDisconnected;
extern Event ClockSynced;

extern StateMachine DataSM;

extern Task SendDataTask;
extern Task SaveDataTask;

extern data_type send_frame(data_type frame);

typedef struct save_data_type {
  data_type data;
  time_type time;
} save_data_type;

void send_live_data();
void save_live_data();
data_type get_data();
inline void save_data(
    data_type data,
    time_type current_time
    );
bool send_data(
    data_type data,
    time_type current_time
    );
void send_stored_data();
inline void set_mux_val(int val);

void data_sm_trans(Event *event_, int *current_state);
void data_sm_run(int *current_state);

#define READ_PIN 3
#define MUX_SIZE 8

//borrar las sgtes lineas antes de mandar todo a prod
#define EX_DATA 0x90FFAA01
#define GET_PIN(i) (EX_DATA << i) & 0b1
#endif
