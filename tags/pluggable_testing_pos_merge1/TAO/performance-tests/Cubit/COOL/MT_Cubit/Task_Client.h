// $Id$

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

#ifndef TASK_CLIENT_H
#define TASK_CLIENT_H

#include <corba/cool.H>
#include "api/api.H"
#include "cubit.H"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include <math.h>

#if defined (CHORUS)
#include "pccTimer.h"
#endif /* CHORUS */

// @@ Should we put this into a more general file, e.g., OS.h?
//
// I will integrate this, together with the sqrt() function when
// the implementation is complete.  --Sergio.
// @@ Sergio, can you please use the ACE_timer_t here rather than #define'ing double?
#if defined (ACE_LACKS_FLOATING_POINT)
#define double ACE_UINT32
#define fabs(X) ((X) >= 0 ? (X) : -(X))
// the following is just temporary, until we finish the sqrt()
// implementation.
#define sqrt(X) (1)
#endif /* ACE_LACKS_FLOATING_POINT */

// Arbitrary generator used by the client to create the numbers to be
// cubed.
inline int
func (unsigned i)
{
  return i - 117;
}

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

  u_int use_chorus_ipc_;
  // flag that indicates that we are going to use Chorus IPC
  // communication mechanism, instead of the TCP/IP protocol stack.
  // This only applies to the CHORUS ClassiX OS.

  char * ior_header_;
  // pointer to the ior part where you specify the type of transport
  // for COOL (i.e. ipc or tcp)

  u_int grain_;
  // this is the granularity of the timing of the CORBA requests. A
  // value of 5 represents that we will take time every 5 requests,
  // instead of the default of every request (1).
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

  int parse_args (int, char **);
  // parses the arguments

  Task_State *ts_;
  // pointer to shared state

};

#endif /* !defined (TASK_CLIENT_H) */
