/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    client.h
//
// = AUTHOR
//    Andy Gokhale, Brian Mendel, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "cubitC.h"
#include "Task_Client.h"
#include "Util_Thread.h"
#include "Timer.h"

#if defined (CHORUS)
#include "pccTimer.h"
#endif /* CHORUS */

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* defined (VME_DRIVER) */

class Client_i : public virtual MT_Priority
{
public:
  Client_i (void);
  //constructor.
  
  ~Client_i (void);
  // destructor.

  int init (int argc, char *argv[]);
  // Initialize the state of <Client_i>.

  void run (void);
  // Run the tests.

  // @@ Naga, can you please add comments for these methods?
  int do_priority_inversion_test (ACE_Thread_Manager *thread_manager);

  int do_thread_per_rate_test (ACE_Thread_Manager *thread_manager);

  int start_servant (Task_State *ts, ACE_Thread_Manager &thread_manager);

  void output_latency (Task_State *ts);

#if defined (VXWORKS)
  void output_taskinfo (void);
#endif /* VXWORKS */

private:
  void init_low_priority (void);
  // Sets the priority to be used for the low priority clients.

  void calc_util_time (void);
  // Calculate the time for one util computation.

  int activate_high_client (ACE_Thread_Manager *thread_manager);
  // Activates the high priority client.

  int activate_low_client (ACE_Thread_Manager *thread_manager);
  // Activates the low priority client.

  int activate_util_thread (void);
  // Activates the utilization thread.

  void print_priority_inversion_stats (void);
  // Prints the results of the tests.

  void print_context_stats (void);
  // Prints the context switch results.

  void print_util_stats (void);
  // Prints the utilization test results.

  void print_latency_stats (void);
  // Prints the latency and jitter results.

  Client *high_priority_client_;
  // Pointer to the high priority client object.

  Client **low_priority_client_;
  // Array to hold pointers to the low priority tasks.

  ACE_High_Res_Timer timer_;
  // Timer for timing the tests.
  
  MT_Priority priority_;
  // Priority helper object.

  Util_Thread *util_thread_;
  // Utilization thread.

  ACE_Thread_Manager util_thread_manager_;
  // Utilization thread manager.

  ACE_timer_t util_task_duration_;
  // Time for one computation of utilization thread.

  Task_State *ts_;
  // Pointer to task state.

  ACE_Sched_Priority high_priority_;
  // Priority used for the high priority client.

  ACE_Sched_Priority low_priority_;
  // Priority used by the low priority clients.

  u_int num_low_priority_;
  // Number of low priority clients

  u_int num_priorities_;
  // Number of priorities used.

  u_int grain_;
  // Granularity of the assignment of the priorities.  Some OSs have
  // fewer levels of priorities than we have threads in our test, so
  // with this mechanism we assign priorities to groups of threads
  // when there are more threads than priorities.

  u_int counter_;
  // @@ Naga, can you please add a comment here?

  char *task_id_;
  // Set a task_id string starting with "@", so we are able to
  // accurately count the number of context switches.

  ACE_Time_Value delta_;
  // elapsed time for the latency tests.

  int argc_;
  // @@ Naga, can you please add a comment here?

  char **argv_;
  // @@ Naga, can you please add a comment here?

  ACE_Thread_Manager client_thread_manager_;
  // Create a separate manager for the client.  This allows the use of
  // its wait () method on VxWorks, without interfering with the
  // server's (global) thread manager.

  ACE_timer_t total_latency_;
  // @@ Naga, can you please add a comment here?

  ACE_timer_t total_latency_high_;
  // @@ Naga, can you please add a comment here?

  ACE_timer_t total_util_task_duration_;
  // @@ Naga, can you please add a comment here?

  u_int context_switch_;
  // Stores the total number of context switches incurred by the
  // program while making CORBA requests

#if (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32)
  ACE_Profile_Timer timer_for_context_switch;
  ACE_Profile_Timer::Rusage usage;
#endif /* (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32) */
};


