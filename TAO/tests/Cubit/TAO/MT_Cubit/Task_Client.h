/* -*- C++ -*- */
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

#if !defined (TASK_CLIENT_H)
#define TASK_CLIENT_H

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "cubitC.h"

#if defined (CHORUS)
#include "pccTimer.h"
#endif /* CHORUS */

#include <math.h>

// @@ Should we put this into a more general file, e.g., OS.h?
//
// I will integrate this, together with the sqrt() function when
// the implementation is complete.  --Sergio.
// @@ Sergio, can you please use the ACE_timer_t type for this instead
// of #define'ing double?!  
#if defined (ACE_LACKS_FLOATING_POINT)
#define double ACE_UINT32
#define fabs(X) ((X) >= 0 ? (X) : -(X))
// the following is just temporary, until we finish the sqrt()
// implementation.
#define sqrt(X) (1)
#endif /* ACE_LACKS_FLOATING_POINT */

#if !defined (ACE_HAS_THREADS)
class NOOP_ACE_Barrier
{
public:
  NOOP_ACE_Barrier (int) {}
  void wait (void) {}
};
#define ACE_Barrier NOOP_ACE_Barrier
#endif /* ACE_HAS_THREADS */

// Arbitrary generator used by the client to create the numbers to be
// cubed.
static inline int
func (u_int i)
{
  return i - 117;
}

enum Cubit_Datatypes
{
  // = The various datatypes the client and the server can exchange.
  CB_OCTET,
  CB_SHORT,
  CB_LONG,
  CB_STRUCT,

  // = Rate constants.
  CB_20HZ_CONSUMER = 0,
  CB_10HZ_CONSUMER = 1,
  CB_5HZ_CONSUMER = 2,
  CB_1HZ_CONSUMER = 3,

  CB_20HZ_CONSUMER_RATE = 20,
  CB_10HZ_CONSUMER_RATE = 10,
  CB_5HZ_CONSUMER_RATE = 5,
  CB_1HZ_CONSUMER_RATE = 1,

  CB_HIGH_PRIORITY_RATE = 20,
  CB_LOW_PRIORITY_RATE = 10
};

class Task_State
{
  // = TITLE
  //     Maintains state common to multiple Cubit clients.
  //
  // = DESCRIPTION
  //     This class maintains state which is common to the potentially
  //     multiple concurrent clients.
public:
  int parse_args (int argc,char **argv);
  // parses the arguments

  ACE_Barrier *barrier_;
  // Barrier for the multiple clients to synchronize after binding to
  // the servants.

  Task_State (int argc, char **argv);
  // Constructor. Takes the command line arguments, which are later
  // passed into ORB_init.

  ~Task_State (void);
  // Destructor

  CORBA::String key_;
  // All cubit objects will have this as prefix to its key.

  u_int loop_count_;
  // Number of times to loop, making calls.

  u_int thread_count_;
  // Number of concurrent clients to create.

  double *latency_;
  // Array to store the latency for every client, indexed by
  // thread-id.

  int *ave_latency_;
  // Int array to store the latencies.

  Cubit_Datatypes datatype_;
  // Which datatype to use to make the calls.

  ACE_SYNCH_MUTEX lock_;
  // Lock to protect access to this object.

  // = Command line arguments.
  int argc_;
  char **argv_;

  u_int thread_per_rate_;
  // Flag for the thread_per_rate test.

  double **global_jitter_array_;
  // This array stores the latency seen by each client for each
  // request, to be used later to compute jitter.

  u_int *count_;
  // This array stores the call count of each thread.  They will not
  // always have the same call count.

  u_int shutdown_;
  // Flag that indicates if we are going to call the shutdown methos
  // for the servant.

  u_int oneway_;
  // Flag that indicates if we are going to use oneway calls instead
  // of two-way.

  u_int use_name_service_;
  // Flag that say if we are using the or not the name service.

  u_int one_to_n_test_;
  // indicates whether we are running the "1 to n" test, which has 1
  // low priority servant and n low priority clients.

  u_int context_switch_test_;
  // flag to run context switch test

  char **iors_;
  // Array of pointers used to hold the ior strings read from the ior file
  // that the server created.

  int iors_count_;
  // count on the number of iors

  char *ior_file_;
  // Name of the filename that the server used to store the iors.

  u_int granularity_;
  // this is the granularity of the timing of the CORBA requests. A
  // value of 5 represents that we will take time every 5 requests,
  // instead of the default of every request (1).

  u_int use_utilization_test_;
  // flag to indicate we are to use the utilization test.  By default
  // we do not use it, because it can cause starvation with real-time
  // threads

  u_int high_priority_loop_count_;
  // Number of times the high priority looped.  We are going to loop
  // as long as there is low priority clients running, so as to
  // maintain high priority traffic as long as low priority traffic is
  // going through.

  ACE_Thread_Semaphore *semaphore_;
  // semaphore in order for the high priority client to keep running
  // as long as the low priority clients are running.  See explanation
  // of "high_priority_loop_count_" member in this class.

  u_int use_multiple_priority_;
  // flag to indicate we are to use multiple priorities for the low
  // priority clients.  By default we use only one priority for all
  // client threads.

  int utilization_task_started_;
  // Indicates whether the utilization task has started.

  ACE_High_Res_Timer timer_;
  // global timer to be started by the utilization task.

  u_int run_server_utilization_test_;
  // flag to indicate we are to run the utilization test of the server.
  // This means we are not sending requests at a determined frequency,
  // but rather "let it rip"!

  u_int util_time_;
  // the amount of time in seconds that the utilization test will run.

  int ready_;
  // ready flag used by the high priority thread to wake up the low
  // priority threads after it's parsed the arguments.

  ACE_SYNCH_MUTEX ready_mtx_;
  // mutex for the condition variable.

  ACE_Condition<ACE_SYNCH_MUTEX> ready_cnd_;
  // condition variable for the low priority threads to wait 
  //until the high priority thread is done with the arguments parsing.

};

class Client : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //     The Cubit client.
  //
  // = DESCRIPTION
  //     This class implements the Cubit Client, which is an active object.
  //     `n' threads execute svc, and make 2way CORBA calls on the server
public:
  Client (ACE_Thread_Manager *, Task_State *ts, u_int id);
  // Constructor, with a pointer to the common task state.

  virtual int svc (void);
  // The thread function.

  double get_high_priority_latency (void);
  double get_low_priority_latency (void);
  double get_high_priority_jitter (void);
  double get_low_priority_jitter (void);
  double get_latency (u_int thread_id);
  double get_jitter (u_int id);
  // Accessors to get the various measured quantities.

private:
  int run_tests (Cubit_ptr,
                 u_int,
                 u_int,
                 Cubit_Datatypes,
                 double frequency);
  // Makes the calls to the servant.

  void put_latency (double *jitter,
                    double latency,
                    u_int thread_id,
                    u_int count);
  // Records the latencies in the <Task_State>.

  int parse_args (int, char **);
  // Parses the arguments.

  Task_State *ts_;
  // Pointer to shared state.

  u_int id_;
  // unique id of the task

  //  CosNaming::NamingContext_var naming_context_;
  // Object reference to the naming service.

  CosNaming::NamingContext_var mt_cubit_context_;
  // Object reference to the cubit context "MT_Cubit".

  TAO_Naming_Client my_name_client_;
  // Naming Client intermediary to naming service stuff
};

#endif /* !defined (TASK_CLIENT_H) */
