/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Brian Mendel
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan and Nagarajan Surendran.
 */
//=============================================================================


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

/**
 * @class Client_i
 *
 * @brief Helper class implementing the Multi-Threaded Cubit client.
 */
class Client_i : public virtual MT_Priority
{
public:
  ///constructor.
  Client_i (void);

  /// destructor.
  virtual ~Client_i (void);

  /// Initialize the state of <Client_i>.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the tests.
  void run (void);

  /// Performs the priority inversion test.
  int do_priority_inversion_test (void);

  /// Performs the test with 4 threads each sending requests at rates
  /// of 20, 10, 5, and 1Hz.
  int do_thread_per_rate_test (void);

  /// Starts the client utilization thread.
  int start_servant (void);

  /// output the latency results for the requests.
  void output_latency (void);

#if defined (VXWORKS)
  /// Outputs the details of this task to a file taskinfo.txt.
  void output_taskinfo (void);
#endif /* VXWORKS */

private:
  /// Sets the priority to be used for the low priority clients.
  void init_low_priority (void);

  /// Calculate the time for one util computation.
  void calc_util_time (void);

  /// Activates the high priority client.
  int activate_high_client (void);

  /// Activates the low priority client.
  int activate_low_client (void);

  /// Activates the utilization thread.
  int activate_util_thread (void);

  /// Prints the results of the tests.
  void print_priority_inversion_stats (void);

  /// Prints the context switch results.
  void print_context_stats (void);

  /// Prints the utilization test results.
  void print_util_stats (void);

  /// Prints the latency and jitter results.
  void print_latency_stats (void);

  /// gets the number of context switches.
  void get_context_switches (void);

  /// Pointer to the high priority client object.
  Client *high_priority_client_;

  /// Array to hold pointers to the low priority tasks.
  Client **low_priority_client_;

  /// Timer for timing the tests.
  ACE_High_Res_Timer timer_;

  /// Priority helper object.
  MT_Priority priority_;

  /// Utilization thread.
  Util_Thread *util_thread_;

  /// Utilization thread manager.
  ACE_Thread_Manager util_thread_manager_;

  /// Time for one computation of utilization thread.
  ACE_timer_t util_task_duration_;

  /// Pointer to task state.
  Task_State *ts_;

  /// Priority used for the high priority client.
  ACE_Sched_Priority high_priority_;

  /// Priority used by the low priority clients.
  ACE_Sched_Priority low_priority_;

  /// Number of low priority clients
  u_int num_low_priority_;

  /// Number of priorities used.
  u_int num_priorities_;

  /**
   * Granularity of the assignment of the priorities.  Some OSs have
   * fewer levels of priorities than we have threads in our test, so
   * with this mechanism we assign priorities to groups of threads
   * when there are more threads than priorities.
   */
  u_int grain_;

  /// counter of the number of priorities used within a grain.
  u_int counter_;

  // Set a task_name string starting with "@", so we are able to
  // accurately count the number of context switches.
  char* task_name_;

  /// elapsed time for the latency tests.
  ACE_Time_Value delta_;

  /// number of command line arguments.
  int argc_;

  /// command line argument array.
  ACE_TCHAR **argv_;

  /**
   * Create a separate manager for the client.  This allows the use of
   * its wait () method on VxWorks, without interfering with the
   * server's (global) thread manager.
   */
  ACE_Thread_Manager client_thread_manager_;

  /// Stores the total number of context switches incurred by the
  /// program while making CORBA requests
  u_int context_switch_;

  /// Thread manager for the servant used for utilization.
  ACE_Thread_Manager server_thread_manager_;

#if (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32)
  ACE_Profile_Timer timer_for_context_switch;
  ACE_Profile_Timer::Rusage usage;
#endif /* (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32) */
};
