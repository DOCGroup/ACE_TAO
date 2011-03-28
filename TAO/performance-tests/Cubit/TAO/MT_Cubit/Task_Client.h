/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Task_Client.h
 *
 *  $Id$
 *
 *  @author Andy Gokhale
 *  @author Sumedh Mungee
 *  @author Sergio Flores-Gaitan
 *  @author and Nagarajan Surendran.
 */
//=============================================================================


#ifndef TASK_CLIENT_H
#define TASK_CLIENT_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Profile_Timer.h"
#include "ace/ARGV.h"
#include "ace/Sched_Params.h"
#include "ace/High_Res_Timer.h"
#include "ace/Containers.h"

#include "cubitC.h"
#include "cubit_i.h"
#include "Globals.h"
#include "Timer.h"

// FUZZ: disable check_for_math_include
#if !defined (__xlC__)
    // Some plaforms define an exception structure in math.h...
# if defined (__GNUG__)
    // And some compilers have this workaround.  Disable it with this
    // #define, to avoid warning about multiple #defines of exception.
#   define _MATH_H_WRAPPER
# endif /* __GNUG__ */
# define exception _math_exception
# include /**/ <math.h>
# undef exception
#else
# include /**/ <math.h>
#endif

#if defined (ACE_HAS_QUANTIFY)
# define START_QUANTIFY quantify_start_recording_data ();
# define STOP_QUANTIFY quantify_stop_recording_data();
# define CLEAR_QUANTIFY quantify_clear_data ();
#else /* ! ACE_HAS_QUANTIFY */
# define START_QUANTIFY
# define STOP_QUANTIFY
# define CLEAR_QUANTIFY
#endif /* ! ACE_HAS_QUANTIFY */


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

typedef ACE_Unbounded_Queue<ACE_timer_t> JITTER_ARRAY;
typedef ACE_Unbounded_Queue_Iterator<ACE_timer_t> JITTER_ARRAY_ITERATOR;

/**
 * @class Task_State
 *
 * @brief Maintains state common to multiple Cubit clients.
 *
 * This class maintains state which is common to the potentially
 * multiple concurrent clients.
 */
class Task_State
{
public:
  /// Constructor.
  Task_State (void);

  /// parses the arguments with the provided argc and argv.
  int parse_args (int argc,ACE_TCHAR **argv);

  /// Destructor
  ~Task_State (void);

  /// Barrier for the multiple clients to synchronize after binding to
  /// the servants.
  ACE_Barrier *barrier_;

  /// All cubit objects will have this as prefix to its key.
  const char *key_;

  /// Number of times to loop, making calls.
  u_int loop_count_;

  /// Number of concurrent clients to create.
  u_int thread_count_;

  /// Array to store the latency for every client, indexed by
  /// thread-id.
  ACE_timer_t *latency_;

  /// Int array to store the latencies.
  int *ave_latency_;

  /// Which datatype to use to make the calls.
  Cubit_Datatypes datatype_;

  /// Lock to protect access to this object.
  TAO_SYNCH_MUTEX lock_;

  /// Flag for the thread_per_rate test.
  u_int thread_per_rate_;

  /// This array stores the latency seen by each client for each
  /// request, to be used later to compute jitter.
  JITTER_ARRAY **global_jitter_array_;

  /// This array stores the call count of each thread.  They will not
  /// always have the same call count.
  u_int *count_;

  /// Flag that indicates if we are going to call the shutdown methos
  /// for the servant.
  u_int shutdown_;

  /// Flag that indicates if we are going to use oneway calls instead
  /// of two-way.
  u_int oneway_;

  /// Ior array used if utilization test is run.
  char *one_ior_;

  /// indicates whether we are running the "1 to n" test, which has 1
  /// low priority servant and n low priority clients.
  u_int one_to_n_test_;

  /// flag to run context switch test
  u_int context_switch_test_;

  /// Array of pointers used to hold the ior strings read from the ior file
  /// that the server created.
  char **iors_;

  /// count on the number of iors
  int iors_count_;

  /// Name of the filename that the server used to store the iors.
  ACE_TCHAR *ior_file_;

  /**
   * this is the granularity of the timing of the CORBA requests. A
   * value of 5 represents that we will take time every 5 requests,
   * instead of the default of every request (1).
   */
  u_int granularity_;

  /**
   * flag to indicate we are to use the utilization test.  By default
   * we do not use it, because it can cause starvation with real-time
   * threads
   */
  u_int use_utilization_test_;

  /**
   * Number of times the high priority looped.  We are going to loop
   * as long as there is low priority clients running, so as to
   * maintain high priority traffic as long as low priority traffic is
   * going through.
   */
  u_int high_priority_loop_count_;

  /**
   * semaphore in order for the high priority client to keep running
   * as long as the low priority clients are running.  See explanation
   * of "high_priority_loop_count_" member in this class.
   */
  ACE_SYNCH_SEMAPHORE *semaphore_;

  /**
   * flag to indicate we are to use multiple priorities for the low
   * priority clients.  By default we use only one priority for all
   * client threads.
   */
  u_int use_multiple_priority_;

  /// global timer to be started by the utilization task.
  ACE_High_Res_Timer timer_;

  /// ready flag used by the high priority thread to wake up the low
  /// priority threads after it's parsed the arguments.
  int ready_;

  /// mutex for the condition variable.
  TAO_SYNCH_MUTEX ready_mtx_;

  /// condition variable for the low priority threads to wait
  ///until the high priority thread is done with the arguments parsing.
  TAO_SYNCH_CONDITION ready_cnd_;

  /// flag to indicate whether we make remote versus local invocations
  /// to calculate accurately the ORB overhead.
  u_int remote_invocations_;

  /// holds the total time for the utilization test to complete.
  ACE_timer_t util_test_time_;
};

/**
 * @class Client
 *
 * @brief The Cubit client.
 *
 * This class implements the Cubit Client, which is an active object.
 * `n' threads execute svc, and make 2way CORBA calls on the server
 */
class Client : public ACE_Task<ACE_SYNCH>
{
public:
  /// Constructor, with a pointer to the common task state.
  Client (ACE_Thread_Manager *,
          Task_State *ts,
          int argc,
          ACE_TCHAR **argv,
          u_int id);

  /// destructor.
  ~Client (void);

  /// The thread function.
  virtual int svc (void);

  /// Returns the latency of the high priority thread in usecs.
  ACE_timer_t get_high_priority_latency (void);

  /// Returns the average latency found for the low
  /// priority threads in usecs.
  ACE_timer_t get_low_priority_latency (void);

  /// Returns the high priority jitter in usecs.
  ACE_timer_t get_high_priority_jitter (void);

  /// Returns the jitter for all the low priority
  /// thread request in usecs.
  ACE_timer_t get_low_priority_jitter (void);

  /// gets the average latency for that thread.
  ACE_timer_t get_latency (u_int thread_id);

  /// gets the jitter for this thread.
  ACE_timer_t get_jitter (u_int id);

  /// Arbitrary generator used by the client to create the numbers to be
  /// cubed.
  static int func (u_int i);

private:
  /// initialize the ORB.
  CORBA::ORB_ptr init_orb (void);

  /// reads the cubit ior from a file.
  void read_ior (void);

  /// gets the cubit object.
  int get_cubit (CORBA::ORB_ptr orb);

  /// Run the various tests.
  int run_tests (void);

  /// make a CORBA request depending on the datatype.
  int make_request (void);

  /// makes the corba requests.
  int do_test (void);

  /// call cube_octet method on the cubit object.
  int cube_octet (void);

  /// call cube short on the cubit object.
  int cube_short (void);

  /// call cube long on the cubit object.
  int cube_long (void);

  /// call cube struct on the cubit object.
  int cube_struct (void);

  /// prints the latency stats.
  void print_stats (void);

  /// Records the latencies in the <Task_State>.
  void put_latency (JITTER_ARRAY *jitter,
                    ACE_timer_t latency,
                    u_int thread_id,
                    u_int count);

  /// Parses the arguments.
  int parse_args (int, char **);

  /// determines the frequency at which to make calls depending on the
  /// id of the thread.
  void find_frequency (void);

  /// calculate the delta value.
  ACE_timer_t calc_delta (ACE_timer_t real_time,
                          ACE_timer_t delta);

  /// pointer to the cubit object.
  Cubit_ptr cubit_;

  /// cubit implementation object.
  Cubit_i cubit_impl_;

  /// Pointer to shared state.
  Task_State *ts_;

  /// number used for cubing.
  u_int num_;

  /// unique id of the task
  u_int id_;

  /// count of the number of calls made.
  u_int call_count_;

  /// number of calls that failed.
  u_int error_count_;

  /// ACE Unbounded set holding the latency values for all the
  /// requests of this thread.
  JITTER_ARRAY *my_jitter_array_;

  /// Timer using pccTimer for chorus and ACE_Timer for other platforms.
  MT_Cubit_Timer *timer_;

  /// frequency of CORBA requests.
  ACE_timer_t frequency_;

  /// aggregate latency of the requests.
  ACE_timer_t latency_;

  // command-line arguments.
  int argc_;
  ACE_TCHAR **argv_;
};

#endif /* !defined (TASK_CLIENT_H) */
