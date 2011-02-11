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
//    Andy Gokhale, Brian Mendel, Sumedh Mungee, Sergio Flores-Gaitan
//    and Nagarajan Surendran.
//
// ============================================================================

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "cubitC.h"
#include "Task_Client.h"
#include "Util_Thread.h"
#include "Timer.h"

#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* defined (VME_DRIVER) */

class Client_i : public virtual MT_Priority
{
  // = TITLE
  //     Helper class implementing the Multi-Threaded Cubit client.
public:
  Client_i (void);
  //constructor.

  virtual ~Client_i (void);
  // destructor.

  int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the state of <Client_i>.

  void run (void);
  // Run the tests.

  int do_priority_inversion_test (void);
  // Performs the priority inversion test.

  int do_thread_per_rate_test (void);
  // Performs the test with 4 threads each sending requests at rates
  // of 20, 10, 5, and 1Hz.

  int start_servant (void);
  // Starts the client utilization thread.

  void output_latency (void);
  // output the latency results for the requests.

#if defined (VXWORKS)
  void output_taskinfo (void);
  // Outputs the details of this task to a file taskinfo.txt.
#endif /* VXWORKS */

private:
  void init_low_priority (void);
  // Sets the priority to be used for the low priority clients.

  void calc_util_time (void);
  // Calculate the time for one util computation.

  int activate_high_client (void);
  // Activates the high priority client.

  int activate_low_client (void);
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

  void get_context_switches (void);
  // gets the number of context switches.

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
  // counter of the number of priorities used within a grain.

  // Set a task_name string starting with "@", so we are able to
  // accurately count the number of context switches.
  char* task_name_;

  ACE_Time_Value delta_;
  // elapsed time for the latency tests.

  int argc_;
  // number of command line arguments.

  ACE_TCHAR **argv_;
  // command line argument array.

  ACE_Thread_Manager client_thread_manager_;
  // Create a separate manager for the client.  This allows the use of
  // its wait () method on VxWorks, without interfering with the
  // server's (global) thread manager.

  u_int context_switch_;
  // Stores the total number of context switches incurred by the
  // program while making CORBA requests

  ACE_Thread_Manager server_thread_manager_;
  // Thread manager for the servant used for utilization.

#if (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32)
  ACE_Profile_Timer timer_for_context_switch;
  ACE_Profile_Timer::Rusage usage;
#endif /* (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32) */
};
