// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    client.cpp
//
// = AUTHOR
//    Andy Gokhale, Brian Mendel, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include "client.h"
#include "ace/Sched_Params.h"

double csw = 0.0;

int
initialize (void)
{
#if defined (VXWORKS) && defined (VME_DRIVER)
  STATUS status = vmeDrv ();
  if (status != OK)
    printf ("ERROR on call to vmeDrv()\n");
  status = vmeDevCreate ("/vme");
  if (status != OK)
    printf ("ERROR on call to vmeDevCreate()\n");
#endif /* VXWORKS && VME_DRIVER */

  // Make sure we've got plenty of socket handles.  This call will
  // use the default maximum.
  ACE::set_handle_limit ();

  return 0;
}

int
do_priority_inversion_test (ACE_Thread_Manager &thread_manager,
                            Task_State &ts)
{
  int i;
  u_int j, k;

  // stores the total number of context switches incurred by the
  // program while making CORBA requests
#if defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)
  u_int context_switch = 0;
#endif /* ACE_HAS_PRUSAGE_T || ACE_HAS_GETRUSAGE */
  double util_task_duration = 0.0;
  double total_latency = 0.0;
  double total_latency_low = 0.0;
  double total_latency_high = 0.0;
  double total_util_task_duration = 0.0;

  // Create the clients.
  Client high_priority_client (thread_manager, &ts, 0);

  // Create an array to hold pointers to the low priority tasks.
  Client **low_priority_client;

  ACE_NEW_RETURN (low_priority_client,
                  Client *[ts.thread_count_],
                  -1);

  // hack to make sure we have something in this pointer, when
  // thread_count == 1
  low_priority_client[0] = &high_priority_client;

  // Create the daemon thread in its own <ACE_Thread_Manager>.
  ACE_Thread_Manager util_thr_mgr;

  Util_Thread util_thread (&ts, &util_thr_mgr);

  //
  // Time the utilization thread' "computation" to get %IdleCPU at the end of the test.
  //
#if defined (CHORUS)
  int pstartTime = 0;
  int pstopTime = 0;
  // Elapsed time will be in microseconds.
  pstartTime = pccTime1Get();
  // execute one computation.
  util_thread.computation ();
  pstopTime = pccTime1Get();
  // Store the time in micro-seconds.
  util_task_duration = pstopTime - pstartTime;
#else /* CHORUS */
  ACE_High_Res_Timer timer_;
  // Elapsed time will be in microseconds.
  ACE_Time_Value delta_t;
  timer_.start ();
  // execute one computation.
  util_thread.computation ();
  timer_.stop ();
  timer_.elapsed_time (delta_t);
  // Store the time in milli-seconds.
  util_task_duration = delta_t.sec () * ACE_ONE_SECOND_IN_MSECS + (double)delta_t.usec () / ACE_ONE_SECOND_IN_MSECS;
#endif /* !CHORUS */

  // The minimum priority thread is the utilization thread.
  ACE_Sched_Priority priority =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);

  if (ts.use_utilization_test_ == 1)
    // activate the utilization thread only if specified.  See
    // description of this variable in header file.
    {
      // activate the Utilization thread.  It will wait until
      // all threads have finished binding.
      util_thread.activate (THR_BOUND | ACE_SCHED_FIFO,
                            1,
                            0,
                            priority);
    }
  else
    {
      util_thread.close ();
    }

  // Now activate the high priority client with default real-time priority
  priority = ACE_THR_PRI_FIFO_DEF;

  ACE_DEBUG ((LM_DEBUG,
              "Creating 1 client with high priority of %d\n",
              priority));

  if (high_priority_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                                     1,
                                     0,
                                     priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p; priority is %d\n",
                "activate failed",
                priority));

  const ACE_pri_t max_low_client_priority = 
    ACE_Sched_Params::previous_priority (
      ACE_SCHED_FIFO,
      ACE_Sched_Params::previous_priority (
        ACE_SCHED_FIFO,
        priority,
        ACE_SCOPE_THREAD),
      ACE_SCOPE_THREAD);


  priority = max_low_client_priority;

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d clients with low priority %d\n",
              ts.thread_count_ - 1,
              priority));

  for (i = ts.thread_count_ - 1; i > 0; i--)
    {
      ACE_NEW_RETURN (low_priority_client [i - 1],
                      Client (thread_manager, &ts, i),
                      -1);

#if defined (VXWORKS)
      // Pace the connection establishment on VxWorks.
      const ACE_Time_Value delay (0L, 500000L);
      ACE_OS::sleep (delay);
#endif /* VXWORKS */

      ACE_DEBUG ((LM_DEBUG,
                  "Creating client with thread ID %d\n",
                  i));

      // The first thread starts at the lowest priority of all the low
      // priority clients.
      if (low_priority_client[i - 1]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                            1,
                                            1,
                                            priority) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p; priority is %d\n",
                    "activate failed",
                    priority));
    }

  // Wait for all the client threads to be initialized before going
  // any further.
  ts.barrier_->wait ();

#if defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)
  ACE_Profile_Timer timer_for_context_switch;
  ACE_Profile_Timer::Rusage usage;
  timer_for_context_switch.start ();
  timer_for_context_switch.get_rusage (usage);
#if defined (ACE_HAS_PRUSAGE_T)
  context_switch = usage.pr_vctx + usage.pr_ictx;
#else  /* ACE_HAS_PRUSAGE_T */
  context_switch = usage.ru_nvcsw + usage.ru_nivcsw;
#endif /* ACE_HAS_GETRUSAGE */
#endif /* ACE_HAS_PRUSAGE_T || ACE_HAS_GETRUSAGE */

  // Wait for all the client threads to exit (except the utilization thread).
  thread_manager.wait ();

  // signal the utilization thread to finish with its work..
  util_thread.done_ = 1;

  // This will wait for the utilization thread to finish.
  util_thr_mgr.wait ();

  ACE_DEBUG ((LM_DEBUG, 
	      "-------------------------- Stats -------------------------------\n"));
#if defined (ACE_HAS_PRUSAGE_T)
  timer_for_context_switch.stop ();
  timer_for_context_switch.get_rusage (usage);
  // Add up the voluntary context switches & involuntary context switches
  context_switch = usage.pr_vctx + usage.pr_ictx - context_switch;
  ACE_DEBUG ((LM_DEBUG, 
	      "Voluntary context switches=%d, Involuntary context switches=%d\n",
	      usage.pr_vctx,
	      usage.pr_ictx));
#elif defined (ACE_HAS_GETRUSAGE)
  timer_for_context_switch.stop ();
  timer_for_context_switch.get_rusage (usage);
  // Add up the voluntary context switches & involuntary context switches
  context_switch = usage.ru_nvcsw + usage.ru_nivcsw - context_switch;
  ACE_DEBUG ((LM_DEBUG, 
	      "Voluntary context switches=%d, Involuntary context switches=%d\n",
	      usage.ru_nvcsw,
	      usage.ru_nivcsw));
#endif /* ACE_HAS_GETRUSAGE */

#if 0 // Disable the calculation of context switch time.  It seems to
      // hang the machine, when running on single-processor machines.
      // Instead, to find the context switch time, run $ACE/performance-tests/Misc/context_switch_time
    csw = context_switch_time ();
#endif

#if defined (VXWORKS)
  ACE_DEBUG ((LM_DEBUG, "Test done.\n"
              "High priority client latency : %f msec, jitter: %f msec\n"
              "Low priority client latency : %f msec, jitter: %f msec\n",
              high_priority_client.get_high_priority_latency (),
              high_priority_client.get_high_priority_jitter (),
              low_priority_client[0]->get_low_priority_latency (),
              low_priority_client[0]->get_low_priority_jitter ()));
  // output the latency values to a file, tab separated, to import it
  // to Excel to calculate jitter, in the mean time we come up with
  // the sqrt() function.
  FILE *latency_file_handle = 0;
  char latency_file[BUFSIZ];
  char buffer[BUFSIZ];

  ACE_OS::sprintf (latency_file,
                   "cb__%d.txt",
                   ts.thread_count_);

  ACE_OS::fprintf(stderr,
                  "--->Output file for latency data is \"%s\"\n",
                  latency_file);

  latency_file_handle = ACE_OS::fopen (latency_file, "w");

  // This loop visits each client.  thread_count_ is the number of clients.
  for (u_int j = 0; j < ts.thread_count_; j ++)
    {
      ACE_OS::sprintf(buffer,
                      "%s #%d",
                      j==0? "High Priority": "Low Priority",
                      j);
      // this loop visits each request latency from a client
      for (u_int i = 0; i < (j==0? ts.high_priority_loop_count_:ts.loop_count_)/ts.granularity_; i ++)
        {
          ACE_OS::sprintf(buffer+strlen(buffer),
                          "\t%f\n",
                          ts.global_jitter_array_[j][i]);
          fputs (buffer, latency_file_handle);
          buffer[0]=0;
        }
    }

  ACE_OS::fclose (latency_file_handle);
#elif defined (CHORUS)
  ACE_DEBUG ((LM_DEBUG, 
	      "Test done.\n"
	      "High priority client latency : %u usec\n"
	      "Low priority client latency : %u usec\n",
	      high_priority_client.get_high_priority_latency (),
	      low_priority_client[0]->get_low_priority_latency () ));

  // output the latency values to a file, tab separated, to import it
  // to Excel to calculate jitter, in the mean time we come up with
  // the sqrt() function.
  FILE *latency_file_handle = 0;
  char latency_file[BUFSIZ];
  char buffer[BUFSIZ];

  ACE_OS::sprintf (latency_file,
                   "cb__%d.txt",
                   ts.thread_count_);

  ACE_OS::fprintf(stderr,
                  "--->Output file for latency data is \"%s\"\n",
                  latency_file);

  latency_file_handle = ACE_OS::fopen (latency_file, "w");

  // This loop visits each client.  thread_count_ is the number of clients.
  for (u_int j = 0; j < ts.thread_count_; j ++)
    {
      ACE_OS::sprintf(buffer,
                      "%s #%d",
                      j==0? "High Priority": "Low Priority",
                      j);
      // this loop visits each request latency from a client
      for (u_int i = 0; i < ts.loop_count_/ts.granularity_; i ++)
        {
          ACE_OS::sprintf(buffer+strlen(buffer),
                          "\t%u\n",
                          ts.global_jitter_array_[j][i]);
          fputs (buffer, latency_file_handle);
          buffer[0]=0;
        }
    }

  ACE_OS::fclose (latency_file_handle);
#else /* !CHORUS */
  ACE_DEBUG ((LM_DEBUG, "Test done.\n"
              "High priority client latency : %f msec, jitter: %f msec\n"
              "Low priority client latency : %f msec, jitter: %f msec\n"
              "# of context switches: %d, context_switch_time: %f msec\n"
              "total context switch time: %f msec\n",
              high_priority_client.get_high_priority_latency (),
              high_priority_client.get_high_priority_jitter (),
              low_priority_client[0]->get_low_priority_latency (),
              low_priority_client[0]->get_low_priority_jitter (),
              context_switch,
              csw/1000,
              csw * context_switch/1000 ));
#endif /* !VXWORKS && !CHORUS */

  // This loop visits each client.  thread_count_ is the number of clients.
  for (j = 1; j < ts.thread_count_; j ++)
      for (k = 0; k < ts.loop_count_/ts.granularity_; k ++)
          total_latency_low += (ts.global_jitter_array_[j][k] * ts.granularity_);

  for (j = 0; j < ts.loop_count_/ts.granularity_; j ++)
    total_latency_high += (ts.global_jitter_array_[0][j] * ts.granularity_);

  total_util_task_duration = util_task_duration * util_thread.get_number_of_computations ();

  total_latency = total_latency_low +
    total_latency_high +
    total_util_task_duration;

  // Calc and print the CPU percentage. I add 0.5 to round to the
  // nearest integer before casting it to int.
  ACE_DEBUG ((LM_DEBUG, 
	      "\t%% Low Priority CPU utilization: %d %%\n"
	      "\t%% High Priority CPU utilization: %d %%\n"
	      "\t%% IDLE time: %d %%\n",
	      (int) (total_latency_low * 100 / total_latency + 0.5),
	      (int) (total_latency_high * 100 / total_latency + 0.5),
	      (int) (total_util_task_duration * 100 / total_latency + 0.5) ));

#if defined (ACE_LACKS_FLOATING_POINT)
  ACE_DEBUG ((LM_DEBUG,
              "(%t) utilization task performed %u computations\n",
              util_thread.get_number_of_computations ()));
  ACE_DEBUG ((LM_DEBUG,
	      "(%t) utilization computation time is %u usecs\n", 
	      util_task_duration));
#else
  ACE_DEBUG ((LM_DEBUG,
              "(%t) utilization task performed %g computations\n",
              util_thread.get_number_of_computations ()));
  ACE_DEBUG ((LM_DEBUG,
	      "(%t) utilization computation time is %f msecs\n", 
	      util_task_duration));
#endif /* ! ACE_LACKS_FLOATING_POINT */
  
  return 0;
}

int
do_thread_per_rate_test (ACE_Thread_Manager &thread_manager,
                         Task_State &ts)
{
  // First activate the high priority client.
    Client CB_40Hz_client (thread_manager, &ts, CB_40HZ_CONSUMER);
    Client CB_20Hz_client (thread_manager, &ts, CB_20HZ_CONSUMER);
    Client CB_10Hz_client (thread_manager, &ts, CB_10HZ_CONSUMER);
    Client CB_5Hz_client (thread_manager, &ts, CB_5HZ_CONSUMER);
    Client CB_1Hz_client (thread_manager, &ts, CB_1HZ_CONSUMER);

    ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF;

    ACE_DEBUG ((LM_DEBUG,
                "Creating 40 Hz client with priority %d\n",
                priority));
    if (CB_40Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, priority) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "activate failed"));

    priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    priority,
                                                    ACE_SCOPE_THREAD);
    ACE_DEBUG ((LM_DEBUG,
                "Creating 20 Hz client with priority %d\n",
                priority));
    if (CB_20Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, priority) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "activate failed"));

    priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    priority,
                                                    ACE_SCOPE_THREAD);
    ACE_DEBUG ((LM_DEBUG,
                "Creating 10 Hz client with priority %d\n",
                priority));
    if (CB_10Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, priority) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "activate failed"));

    priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    priority,
                                                    ACE_SCOPE_THREAD);
    ACE_DEBUG ((LM_DEBUG,
                "Creating 5 Hz client with priority %d\n",
                priority));
    if (CB_5Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, priority) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "activate failed"));

    priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                    priority,
                                                    ACE_SCOPE_THREAD);
    ACE_DEBUG ((LM_DEBUG,
                "Creating 1 Hz client with priority %d\n",
                priority));
    if (CB_1Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO, 1, 0, priority) == -1)
      ACE_ERROR ((LM_ERROR,
                  "%p\n",
                  "activate failed"));

    // Wait for all the threads to exit.
    ACE_Thread_Manager::instance ()->wait ();

    ACE_DEBUG ((LM_DEBUG,
                "Test done.\n"
                "40Hz client latency : %u usec\n"
                "20Hz client latency : %u usec\n"
                "10Hz client latency : %u usec\n"
                "5Hz client latency : %u usec\n"
                "1Hz client latency : %u usec\n",
                CB_40Hz_client.get_latency (0),
                CB_20Hz_client.get_latency (1),
                CB_10Hz_client.get_latency (2),
                CB_5Hz_client.get_latency (3),
                CB_1Hz_client.get_latency (4)));
    return 0;
}

// This is the main routine of the client, where we create a high
// priority and a low priority client. we then activate the clients
// with the appropriate priority threads, and wait for them to
// finish. After they aer done, we compute the latency and jitter
// metrics and print them.

#if defined (VXWORKS)
extern "C"
int
client (int argc, char *argv[])
{
  ACE_Object_Manager ace_object_manager;
#else
int
main (int argc, char *argv[])
{
#endif

#if defined (ACE_HAS_THREADS)
#if defined (FORCE_ARGS)
  int argc = 4;
  char *argv[] = {"client",
                  "-s",
                  "-f",
                  "ior.txt"};
#endif   /* defined (FORCE_ARGS) */

  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
#if defined (__Lynx__)
          30,
#else  /* ! __Lynx__ */
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
#endif /* ! __Lynx__ */
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_MAX, "preempt: user is not superuser, "
                    "so remain in time-sharing class\n"));
      else
        ACE_ERROR_RETURN ((LM_ERROR, "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }

  initialize ();

  Task_State ts (argc, argv);

#if defined (CHORUS)
  // start the pccTimer for chorus classix
  int pTime;

  // Initialize the PCC timer Chip
  pccTimerInit();

  if(pccTimer(PCC2_TIMER1_START,&pTime) !=K_OK)
    {
      printf("pccTimer has a pending benchmark\n");
    }
#endif /* CHORUS */

  // Create a separate manager for the client.  This allows the use
  // of its wait () method on VxWorks, without interfering with the
  // server's (global) thread manager.
  ACE_Thread_Manager client_thread_manager;

  if (ts.thread_per_rate_ == 0)
    do_priority_inversion_test (client_thread_manager, ts);
  else
    do_thread_per_rate_test (client_thread_manager, ts);

#if defined (CHORUS)
  if(pccTimer(PCC2_TIMER1_STOP,&pTime) !=K_OK)
    {
      printf("pccTimer has a pending bench mark\n");
    }
#elif defined (VXWORKS)
  // Shoot myself.  Otherwise, there's a General Protection Fault.  This
  // will leak memory, but that's preferable.  It looks like the problem
  // might be due to static objects in libTAO or liborbsvcs?
  int status;
  ACE_OS::thr_exit (&status);
#endif /* CHORUS */

#else /* !ACE_HAS_THREADS */
  ACE_DEBUG ((LM_DEBUG,
              "Test not run.  This platform doesn't seem to have threads.\n"));
#endif /* ACE_HAS_THREADS */

  return 0;
}
