// $Id$

#include "pbroker/corba/orb.h"
#include "pbroker/corba/request.h"
#include "pbroker/corba/environ.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "cubit.h"
#include "ace/Profile_Timer.h"
#include <math.h>

inline int func (unsigned i) { return i - 117; }

enum Cubit_Datatypes {CB_OCTET, CB_SHORT, CB_LONG, CB_STRUCT};

#define CB_HIGH_PRIORITY_RATE 20
#define CB_LOW_PRIORITY_RATE 10

class Task_State
{
 public:
  ACE_Barrier *barrier_;
  Task_State (int argc, char **argv);
  unsigned int start_count_;
  unsigned int loop_count_;
  unsigned int thread_count_;
  unsigned int base_port_;
  char server_host_ [1024];
  double *latency_;
  Cubit_Datatypes datatype_;
  ACE_Thread_Mutex lock_;
  int argc_;
  char **argv_;
};

class Client : public ACE_Task<ACE_MT_SYNCH>
{
public:
  //  Client (char *hostname, unsigned int n_threads, unsigned int loop_count_);
  Client (Task_State *ts);
  virtual int svc (void);
  double get_high_priority_latency ();
  double get_low_priority_latency ();
private:
  int run_tests (Cubit_ptr, unsigned int, unsigned int, Cubit_Datatypes, double frequency);
  void put_latency (double latency, unsigned int);
  int parse_args (int, char **);
  Task_State *ts_;
};
