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
//    Andy Gokhale, Sumedh Mungee, Sergio Flores-Gaitan, and
//    Nagarajan Surendran.
//
// ============================================================================

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

class Task_State
{
  // = TITLE
  //     Maintains state common to multiple Cubit clients.
  //
  // = DESCRIPTION
  //     This class maintains state which is common to the potentially
  //     multiple concurrent clients.
public:
  Task_State (void);
  // Constructor.

  int parse_args (int argc,ACE_TCHAR **argv);
  // parses the arguments with the provided argc and argv.

  ~Task_State (void);
  // Destructor

  ACE_Barrier *barrier_;
  // Barrier for the multiple clients to synchronize after binding to
  // the servants.

  const char *key_;
  // All cubit objects will have this as prefix to its key.

  u_int loop_count_;
  // Number of times to loop, making calls.

  u_int thread_count_;
  // Number of concurrent clients to create.

  ACE_timer_t *latency_;
  // Array to store the latency for every client, indexed by
  // thread-id.

  int *ave_latency_;
  // Int array to store the latencies.

  Cubit_Datatypes datatype_;
  // Which datatype to use to make the calls.

  TAO_SYNCH_MUTEX lock_;
  // Lock to protect access to this object.

  u_int thread_per_rate_;
  // Flag for the thread_per_rate test.

  JITTER_ARRAY **global_jitter_array_;
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

  char *one_ior_;
  // Ior array used if utilization test is run.

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

  ACE_TCHAR *ior_file_;
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

  ACE_SYNCH_SEMAPHORE *semaphore_;
  // semaphore in order for the high priority client to keep running
  // as long as the low priority clients are running.  See explanation
  // of "high_priority_loop_count_" member in this class.

  u_int use_multiple_priority_;
  // flag to indicate we are to use multiple priorities for the low
  // priority clients.  By default we use only one priority for all
  // client threads.

  ACE_High_Res_Timer timer_;
  // global timer to be started by the utilization task.

  int ready_;
  // ready flag used by the high priority thread to wake up the low
  // priority threads after it's parsed the arguments.

  TAO_SYNCH_MUTEX ready_mtx_;
  // mutex for the condition variable.

  TAO_SYNCH_CONDITION ready_cnd_;
  // condition variable for the low priority threads to wait
  //until the high priority thread is done with the arguments parsing.

  u_int remote_invocations_;
  // flag to indicate whether we make remote versus local invocations
  // to calculate accurately the ORB overhead.

  ACE_timer_t util_test_time_;
  // holds the total time for the utilization test to complete.
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
  Client (ACE_Thread_Manager *,
          Task_State *ts,
          int argc,
          ACE_TCHAR **argv,
          u_int id);
  // Constructor, with a pointer to the common task state.

  ~Client (void);
  // destructor.

  virtual int svc (void);
  // The thread function.

  ACE_timer_t get_high_priority_latency (void);
  // Returns the latency of the high priority thread in usecs.

  ACE_timer_t get_low_priority_latency (void);
  // Returns the average latency found for the low
  // priority threads in usecs.

  ACE_timer_t get_high_priority_jitter (void);
  // Returns the high priority jitter in usecs.

  ACE_timer_t get_low_priority_jitter (void);
  // Returns the jitter for all the low priority
  // thread request in usecs.

  ACE_timer_t get_latency (u_int thread_id);
  // gets the average latency for that thread.

  ACE_timer_t get_jitter (u_int id);
  // gets the jitter for this thread.

  static int func (u_int i);
  // Arbitrary generator used by the client to create the numbers to be
  // cubed.

private:
  CORBA::ORB_ptr init_orb (void);
  // initialize the ORB.

  void read_ior (void);
  // reads the cubit ior from a file.

  int get_cubit (CORBA::ORB_ptr orb);
  // gets the cubit object.

  int run_tests (void);
  // Run the various tests.

  int make_request (void);
  // make a CORBA request depending on the datatype.

  int do_test (void);
  // makes the corba requests.

  int cube_octet (void);
  // call cube_octet method on the cubit object.

  int cube_short (void);
  // call cube short on the cubit object.

  int cube_long (void);
  // call cube long on the cubit object.

  int cube_struct (void);
  // call cube struct on the cubit object.

  void print_stats (void);
  // prints the latency stats.

  void put_latency (JITTER_ARRAY *jitter,
                    ACE_timer_t latency,
                    u_int thread_id,
                    u_int count);
  // Records the latencies in the <Task_State>.

  int parse_args (int, char **);
  // Parses the arguments.

  void find_frequency (void);
  // determines the frequency at which to make calls depending on the
  // id of the thread.

  ACE_timer_t calc_delta (ACE_timer_t real_time,
                          ACE_timer_t delta);
  // calculate the delta value.

  Cubit_ptr cubit_;
  // pointer to the cubit object.

  Cubit_i cubit_impl_;
  // cubit implementation object.

  Task_State *ts_;
  // Pointer to shared state.

  u_int num_;
  // number used for cubing.

  u_int id_;
  // unique id of the task

  u_int call_count_;
  // count of the number of calls made.

  u_int error_count_;
  // number of calls that failed.

  JITTER_ARRAY *my_jitter_array_;
  // ACE Unbounded set holding the latency values for all the
  // requests of this thread.

  MT_Cubit_Timer *timer_;
  // Timer using pccTimer for chorus and ACE_Timer for other platforms.

  ACE_timer_t frequency_;
  // frequency of CORBA requests.

  ACE_timer_t latency_;
  // aggregate latency of the requests.

  // command-line arguments.
  int argc_;
  ACE_TCHAR **argv_;
};

#endif /* !defined (TASK_CLIENT_H) */
