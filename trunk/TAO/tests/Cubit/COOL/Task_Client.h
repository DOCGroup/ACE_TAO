#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "api/api.H"
#include "cubit.H"

inline int func (unsigned i) { return i - 117; }

class Client // : ACE_Task<ACE_MT_SYNCH> 
{
public:
  Client (char *hostname, 
          unsigned int n_threads, 
          unsigned int loop_count_);
  int svc (void);
  double get_high_priority_latency ();
  double get_low_priority_latency ();
private:
  ACE_Barrier *barrier_;
  void run_tests (Cubit_ptr, int, unsigned int);
  void put_latency (long latency, unsigned int);
  unsigned int start_count_;
  unsigned int loop_count_;
  char *hostname_;
  long *latency_;
  Cubit_ptr cb_;
  ACE_Thread_Mutex lock_;
};
