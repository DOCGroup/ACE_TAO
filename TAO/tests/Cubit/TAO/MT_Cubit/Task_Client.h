// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    Task_Client.h
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#if !defined (TASK_CLIENT_H)
#define TASK_CLIENT_H

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "cubitC.h"
#include "ace/Profile_Timer.h"
#include <math.h>

// Arbitrary generator used by the client to create the numbers to be
// cubed.
inline int func (unsigned i) { return i - 117; }

// The various datatypes which the client and the server can exchange. 
enum Cubit_Datatypes 
{
  CB_OCTET,
  CB_SHORT,
  CB_LONG,
  CB_STRUCT
};

const int CB_40HZ_CONSUMER = 0;
const int CB_20HZ_CONSUMER = 1;    
const int CB_10HZ_CONSUMER = 2;    
const int CB_5HZ_CONSUMER = 3;    
const int CB_1HZ_CONSUMER = 4;

const int CB_40HZ_CONSUMER_RATE = 40;
const int CB_20HZ_CONSUMER_RATE = 20;
const int CB_10HZ_CONSUMER_RATE = 10;
const int CB_5HZ_CONSUMER_RATE = 5;
const int CB_1HZ_CONSUMER_RATE = 1;

const int CB_HIGH_PRIORITY_RATE = 20;
const int CB_LOW_PRIORITY_RATE = 10;

class Task_State
  // = TITLE
  //     Maintains state common to multiple Cubit clients

  // = DESCRIPTION
  //     This class maintains state which is common to the potentially
  //     multiple concurrent clients. 


{
public:
  ACE_Barrier *barrier_;
  // Barrier for the multiple clients to synchronize after
  // binding to the servants.

  Task_State (int argc, char **argv);
  // Constructor. Takes the command line arguments, which are
  // later passed into ORB_init

  u_int start_count_;
  // keeps a count of the number of clients started.. This
  // count also serves as a thread-id. The first thread created
  // gets an ID of 0, then 1, and so on..

  u_int loop_count_;
  // number of times to loop, making calls..

  u_int thread_count_;
  // number of concurrent clients to create..

  u_int base_port_;
  // this is the port at which the high priority servant is
  // listening.. lower priority ports begin at base_port_ + 1

  char server_host_ [1024];
  // Server hostname

  double *latency_;
  // Array to store the latency for every client, indexed by
  // thread-id.

  int *ave_latency_;
  // Int array to store the latencies

  Cubit_Datatypes datatype_;
  // Which datatype to use to make the calls

  ACE_Thread_Mutex lock_;
  // lock to protect access to this object

  int argc_;
  char **argv_;
  // command line arguments

  u_int thread_per_rate_;
  // Flag for the thread_per_rate test

  double **global_jitter_array_;
  // this array stores the latency seen by each client for each
  // request, to be used later to compute jitter

};

class Client : public ACE_Task<ACE_MT_SYNCH>
  // = TITLE
  //     The Cubit client. 
{
public:
  Client (Task_State *ts);
  // constructor, with a pointer to the common task state.

  virtual int svc (void);
  // the thread function

  double get_high_priority_latency (void);
  double get_low_priority_latency (void);
  double get_high_priority_jitter (void);
  double get_low_priority_jitter (void);
  int get_latency (u_int thread_id);
  // accessors to get the various measured quantities


private:

  int run_tests (Cubit_ptr, 
                 unsigned int, 
                 unsigned int, 
                 Cubit_Datatypes, 
                 double frequency);
  // makes the calls to the servant

  void put_latency (double *jitter, 
                    double latency, 
                    unsigned int);
  // Records the latencies in the Task_State

  void put_ave_latency (int latency, unsigned int);
  // Records the latencies in the Task_State 

  int parse_args (int, char **);
  // parses the arguments

  Task_State *ts_;
  // pointer to shared state
  
};

#endif /* !defined (TASK_CLIENT_H) */
