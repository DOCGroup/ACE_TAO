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

#include "ace/Sched_Params.h"
#include "server.h"
#include "client.h"
#include "Globals.h"

#if defined (NO_ACE_QUANTIFY)
#include "quantify.h"
#endif /* NO_ACE_QUANTIFY */

ACE_RCSID(MT_Cubit, client, "$Id$")

#if defined (FORCE_ARGS)
char *force_argv[] = {"client",
                      "-s",
                      "-f",
                      "ior.txt"};
#endif /* FORCE_ARGS */

#if defined (VXWORKS)
u_int ctx = 0;
u_int ct = 0;

typedef struct
{
  char name[15];
  WIND_TCB *tcb;
  INSTR *pc;
} task_info;

const int SWITCHES=25000;
task_info tInfo[SWITCHES];

extern "C"
int
switchHook (WIND_TCB *pOldTcb,    // pointer to old task's WIND_TCB.
            WIND_TCB *pNewTcb)   // pointer to new task's WIND_TCB.
{
  // We create the client threads with names starting with "@".
  if (pNewTcb->name[0] == '@')
    ctx++;

  if (ct < SWITCHES)
    {
      ACE_OS::strncpy (tInfo[ct].name,
                       pNewTcb->name,
                       TASKNAME_LEN);
      tInfo[ct].tcb = pNewTcb;
      tInfo[ct].pc  = pNewTcb->regs.pc;
      ct++;
    }

  return 0;
}
#endif /* VXWORKS */

// Constructor.

Client_i::Client_i (void)
  : high_priority_client_ (0),
    low_priority_client_ (0),
    util_thread_ (0),
    ts_ (0),
    num_low_priority_ (0),
    num_priorities_ (0),
    grain_ (0),
    counter_ (0),
    task_id_ (0),
    argc_ (0),
    argv_ (0),
    total_latency_ (0),
    total_latency_high_ (0),
    total_util_task_duration_ (0),
    context_switch_ (0)
{
}

// Destructor.

Client_i::~Client_i (void)
{
  if (this->low_priority_client_ != 0)
    // Delete the low priority task array.
    for (u_int i = this->num_low_priority_;
         i > 0;
         i--)
      delete this->low_priority_client_[i - 1];
}

int
Client_i::init (int argc, char *argv[])
{
#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
#if defined (__Lynx__)
                                              30,
#elif defined (VXWORKS) /* ! __Lynx__ */
                                              6,
#elif defined (ACE_WIN32)
                                              ACE_Sched_Params::priority_max 
                                                (ACE_SCHED_FIFO, ACE_SCOPE_THREAD),
#else
                                              ACE_THR_PRI_FIFO_DEF + 25,
#endif /* ! __Lynx__ */
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        ACE_DEBUG ((LM_MAX,
                    "preempt: user is not superuser, "
                    "so remain in time-sharing class\n"));
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%n: ACE_OS::sched_params failed\n%a"),
                          -1);
    }
#else
  ACE_ERROR_RETURN ((LM_ERROR,
                     "Test will not run.  This platform doesn't seem to have threads.\n"),
                    -1);
#endif /* ACE_HAS_THREADS */

  this->argc_ = argc;
  this->argv_ = argv;

  VX_VME_INIT;

#if defined (VXWORKS) && defined (FORCE_ARGS)
  this->argc_ = 4;
  this->argv_ = force_argv;
#endif /* VXWORKS && FORCE_ARGS */
  // Make sure we've got plenty of socket handles.  This call will use
  // the default maximum.
  ACE::set_handle_limit ();

  ACE_NEW_RETURN (this->ts_,
                  Task_State (this->argc_,
                              this->argv_),
                  -1);

  // Preliminary argument processing.
  for (int i=0;
       i< this->argc_;
       i++)
    {
      if (ACE_OS::strcmp (this->argv_[i],"-r") == 0)
        this->ts_->thread_per_rate_ = 1;
      else if (ACE_OS::strcmp (this->argv_[i],"-t") == 0
               && (i - 1 < this->argc_))
        this->ts_->thread_count_ = 
          ACE_OS::atoi (this->argv_[i+1]);
    }
#if defined (CHORUS)
  // start the pccTimer for Chorus ClassiX.
  int pTime;

  // Initialize the PCC timer Chip
  pccTimerInit ();

  if(pccTimer(PCC2_TIMER1_START,
              &pTime) != K_OK)
    ACE_DEBUG ((LM_DEBUG,
                "pccTimer has a pending benchmark\n"));
#endif /* CHORUS */
  return 0;
}

void
Client_i::run (void)
{
  if (this->ts_->thread_per_rate_ == 0)
    this->do_priority_inversion_test (&this->client_thread_manager_);
  else
    this->do_thread_per_rate_test (&this->client_thread_manager_);
}

#if defined (VXWORKS)
void
Client_i::output_taskinfo (void)
{
  FILE *file_handle = ACE_OS::fopen ("taskinfo.txt", "w");

  if (file_handle == 0)
    ACE_ERROR ((LM_ERROR, 
                "%p\n",
                "open"));

  ACE_DEBUG ((LM_DEBUG,
              "--->Output file for taskinfo data is \"taskinfo.txt\"\n"));

  // This loop visits each client.  thread_count_ is the number of
  // clients.

  for (u_int j = 0; j < SWITCHES; j ++)
    ACE_OS::fprintf(file_handle,
                    "\tname= %s\ttcb= %p\tpc= %p\n",
                    tInfo[j].name,
                    tInfo[j].tcb,
                    tInfo[j].pc);

  ACE_OS::fclose (file_handle);
}
#endif /* VXWORKS */

void
Client_i::get_context_switches (void)
{
#if (defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)) && !defined (ACE_WIN32)

  if (this->ts_->context_switch_test_ == 1)
    {
      this->timer_for_context_switch.start ();
      this->timer_for_context_switch.get_rusage (this->usage);
# if defined (ACE_HAS_PRUSAGE_T)
      this->context_switch_ = this->usage.pr_vctx + this->usage.pr_ictx;
# else  /* ACE_HAS_PRUSAGE_T */
      this->context_switch_ = this->usage.ru_nvcsw + this->usage.ru_nivcsw;
# endif /* ACE_HAS_GETRUSAGE */
    }
#endif /* ACE_HAS_PRUSAGE_T || ACE_HAS_GETRUSAGE */

#if defined (VXWORKS)
  if (this->ts_->context_switch_test_ == 1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Adding the context switch hook!\n"));
      taskSwitchHookAdd ((FUNCPTR) &switchHook);
    }
#endif /* VXWORKS */
}

void
Client_i::output_latency (Task_State *ts)
{
  FILE *latency_file_handle = 0;
  char latency_file[BUFSIZ];
  char buffer[BUFSIZ];

  ACE_OS::sprintf (latency_file,
                   "cb__%d.txt",
                   this->ts_->thread_count_);

  ACE_DEBUG ((LM_DEBUG,
              "--->Output file for latency data is \"%s\"\n",
              latency_file));

  latency_file_handle = ACE_OS::fopen (latency_file, "w");

  // This loop visits each client.  thread_count_ is the number of
  // clients.
  for (u_int j = 0;
       j < this->ts_->thread_count_;
       j++)
    {
      ACE_OS::sprintf(buffer,
                      "%s #%d",
                      j == 0
                      ? "High Priority"
                      : "Low Priority",
                      j);
      // This loop visits each request latency from a client.
      for (u_int i = 0;
           i < (j == 0 
                ? this->ts_->high_priority_loop_count_ 
                : this->ts_->loop_count_) / this->ts_->granularity_;
           i++)
        {
          ACE_OS::sprintf (buffer + ACE_OS::strlen (buffer),
#if defined (CHORUS)
                          "\t%u\n",
#else
                          "\t%f\n",
#endif /* !CHORUS */
                           this->ts_->global_jitter_array_[j][i]);
          ACE_OS::fputs (buffer,
                         latency_file_handle);
          buffer[0] = 0;
        }
    }

  ACE_OS::fclose (latency_file_handle);
}

// Mechanism to distribute the available priorities among the threads
// when there are not enough different priorities for all threads.

void
Client_i::init_low_priority (void)
{
  if (this->ts_->use_multiple_priority_ == 1)
    this->low_priority_ =
      this->priority_.get_low_priority (this->high_priority_,
                                        this->num_low_priority_,
                                        1);
  else
    this->low_priority_ =
      this->priority_.get_low_priority (this->high_priority_,
                                        this->num_low_priority_,
                                        0);
  this->num_priorities_ =
    this->priority_.number_of_priorities ();
  this->grain_ = this->priority_.grain ();
  this->counter_ = 0;
}  

void
Client_i::calc_util_time (void)
{
  MT_Cubit_Timer timer (ACE_ONE_SECOND_IN_MSECS);
  // Time the utilization thread' "computation" to get % IdleCPU at the
  // end of the test.

  // Execute one computation.
  timer.start ();
#if defined (CHORUS)
  this->util_thread_->computation ();
  timer.stop ();
  this->util_task_duration_ = timer.get_elapsed ();
#else
  for (u_int i = 0;
       i < 10000;
       i++)
    this->util_thread_->computation ();

  timer.stop ();
  this->util_task_duration_ = timer.get_elapsed () / 10000;
#endif /* !CHORUS */
}

int
Client_i::activate_high_client (ACE_Thread_Manager *thread_manager)
{
  ACE_NEW_RETURN (this->high_priority_client_,
                  Client (thread_manager,
                          this->ts_,
                          0),
                  -1);

#if defined (VXWORKS)
  // Set a task_id string starting with "@", so we are able to
  // accurately count the number of context switches.
  ACE_OS::strcpy (this->task_id_,
                  "@High");
#endif /* VXWORKS */

  this->high_priority_ =
    this->priority_.get_high_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "Creating 1 client with high priority of %d\n",
              this->high_priority_));
  if (this->high_priority_client_->activate (THR_BOUND | ACE_SCHED_FIFO,
                                             1,
                                             0,
                                             this->high_priority_,
                                             -1,
                                             0,
                                             0,
                                             0,
                                             0) == -1)
    // (ACE_thread_t *) this->task_id_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p; priority is %d\n",
                       "activate failed",
                       this->high_priority_),
                      -1);
  return 0;
}

int
Client_i::activate_low_client (ACE_Thread_Manager *thread_manager)
{
  ACE_NEW_RETURN (this->low_priority_client_,
                  Client *[this->ts_->thread_count_],
                  -1);

  // Hack to make sure we have something in this pointer, when
  // thread_count == 1.
  this->low_priority_client_[0] =
    this->high_priority_client_;
  this->num_low_priority_ =
    this->ts_->thread_count_ - 1;

  // Set the priority for the low priority threads.
  this->init_low_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d clients at priority %d\n",
              this->ts_->thread_count_ - 1,
              this->low_priority_));

  for (u_int i = this->num_low_priority_;
       i > 0;
       i--)
    {
      ACE_NEW_RETURN (this->low_priority_client_ [i - 1],
                      Client (thread_manager,
                              this->ts_,
                              i),
                      -1);
#if defined (VXWORKS)
      // Pace the connection establishment on VxWorks.
      const ACE_Time_Value delay (0L, 500000L);
      ACE_OS::sleep (delay);

      // Set a task_id string startiing with "@", so we are able to
      // accurately count the number of context switches on VXWORKS
      sprintf (this->task_id_,
               "@Low%u",
               i);
#endif /* VXWORKS */
      ACE_DEBUG ((LM_DEBUG,
                  "Creating client with thread ID %d and priority %d\n",
                  i,
                  this->low_priority_));
      // The first thread starts at the lowest priority of all the low
      // priority clients.
      if (this->low_priority_client_[i - 1]->activate 
          (THR_BOUND | ACE_SCHED_FIFO,
           1,
           0,
           this->low_priority_,    // These are constructor defaults.
           -1,          //  int grp_id = -1,
           0,           //  ACE_Task_Base *task = 0,
           0,           //  ACE_hthread_t thread_handles[] = 0,
           0,           //  void *stack[] = 0,
           0,           //  size_t stack_size[] = 0,
           (ACE_thread_t *) this->task_id_) == -1)
        ACE_ERROR ((LM_ERROR,
                    "%p; priority is %d\n",
                    "activate failed",
                    this->low_priority_));

      if (this->ts_->use_multiple_priority_ == 1)
        {
          this->counter_ = (this->counter_ + 1) % this->grain_;

          if (this->counter_ == 0 
              // Just so when we distribute the priorities among the
              // threads, we make sure we don't go overboard.
              && this->num_priorities_ * this->grain_ > this->num_low_priority_ - (i - 1))
            // Get the next higher priority.
            this->low_priority_ = ACE_Sched_Params::next_priority 
              (ACE_SCHED_FIFO, this->low_priority_, ACE_SCOPE_THREAD);
        }
    } /* end of for () */

  return 0;
}

int
Client_i::activate_util_thread (void)
{
  ACE_NEW_RETURN (this->util_thread_,
                  Util_Thread (this->ts_,
                               &this->util_thread_manager_),
                  -1);

  // Time the utilization thread' "computation" to get %IdleCPU at the
  // end of the test.
  this->calc_util_time ();

  if (this->ts_->use_utilization_test_ == 1)
    // Activate the utilization thread only if specified.  See
    // description of this variable in header file.
    {
      this->low_priority_ =
        ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                        ACE_SCOPE_THREAD);
      ACE_DEBUG ((LM_DEBUG,
                  "Creating utilization thread with priority of %d\n",
                  this->low_priority_));

      // Activate the Utilization thread.  It will wait until all
      // threads have finished binding.
      this->util_thread_->activate 
        (THR_BOUND | ACE_SCHED_FIFO,
         1,
         0,
         this->low_priority_);
    }
  else
    this->util_thread_->close ();
  return 0;
}

void
Client_i:: print_context_stats (void)
{
  if (this->ts_->context_switch_test_ == 1)
    {
#if defined (ACE_HAS_PRUSAGE_T)
      this->timer_for_context_switch.stop ();
      this->timer_for_context_switch.get_rusage (this->usage);
      // Add up the voluntary context switches & involuntary context
      // switches.
      this->context_switch_ =
        this->usage.pr_vctx + this->usage.pr_ictx - this->context_switch_;
      ACE_DEBUG ((LM_DEBUG,
                  "Voluntary context switches = %d, Involuntary context switches = %d\n",
                  this->usage.pr_vctx,
                  this->usage.pr_ictx));
#elif defined (ACE_HAS_GETRUSAGE) && !defined (ACE_WIN32)
      this->timer_for_context_switch.stop ();
      this->timer_for_context_switch.get_rusage (this->usage);
      // Add up the voluntary context switches & involuntary context
      // switches.
      this->context_switch_ =
        this->usage.ru_nvcsw + this->usage.ru_nivcsw - this->context_switch_;
      ACE_DEBUG ((LM_DEBUG,
                  "Voluntary context switches=%d, Involuntary context switches=%d\n",
                  this->usage.ru_nvcsw,
                  this->usage.ru_nivcsw));
#elif defined (VXWORKS) /* ACE_HAS_GETRUSAGE */
      taskSwitchHookDelete ((FUNCPTR) &switchHook);
      ACE_DEBUG ((LM_DEBUG,
                  "Context switches=%d\n",
                  ctx));
#endif /* ACE_HAS_PRUSAGE_T */
    }
}

void
Client_i::print_latency_stats (void)
{
  // If running the utilization test, don't report latency nor jitter.
  if (this->ts_->use_utilization_test_ == 0)
    {
#if defined (VXWORKS)
      ACE_DEBUG ((LM_DEBUG,
                  "Test done.\n"
                  "High priority client latency : %f msec, jitter: %f msec\n"
                  "Low priority client latency : %f msec, jitter: %f msec\n",
                  this->high_priority_client_->get_high_priority_latency (),
                  this->high_priority_client_->get_high_priority_jitter (),
                  this->low_priority_client_[0]->get_low_priority_latency (),
                  this->low_priority_client_[0]->get_low_priority_jitter ()));
      // Output the latency values to a file, tab separated, to import
      // it to Excel to calculate jitter, in the mean time we come up
      // with the sqrt() function.
      output_latency (this->ts_);
#elif defined (CHORUS)
      ACE_DEBUG ((LM_DEBUG,
                  "Test done.\n"
                  "High priority client latency : %u usec\n"
                  "Low priority client latency : %u usec\n",
                  this->high_priority_client_->get_high_priority_latency (),
                  this->low_priority_client_[0]->get_low_priority_latency () ));
      // Output the latency values to a file, tab separated, to import
      // it to Excel to calculate jitter, in the mean time we come up
      // with the sqrt() function.
      output_latency (this->ts_);
#else /* !CHORUS */
      ACE_DEBUG ((LM_DEBUG, "Test done.\n"
                  "High priority client latency : %f msec, jitter: %f msec\n"
                  "Low priority client latency : %f msec, jitter: %f msec\n",
                  this->high_priority_client_->get_high_priority_latency (),
                  this->high_priority_client_->get_high_priority_jitter (),
                  this->low_priority_client_[0]->get_low_priority_latency (),
                  this->low_priority_client_[0]->get_low_priority_jitter ()));
      // output_latency (this->ts_);
#endif /* !VXWORKS && !CHORUS */
    }
}

void
Client_i::print_util_stats (void)
{
 if (this->ts_->use_utilization_test_ == 1)
    {
      this->total_util_task_duration_ =
        this->util_task_duration_ * this->util_thread_->get_number_of_computations ();

      this->total_latency_ =
        (this->delta_.sec () *
         ACE_ONE_SECOND_IN_MSECS +
         (ACE_timer_t)this->delta_.usec () / ACE_ONE_SECOND_IN_MSECS);

      this->total_latency_high_ =
        this->total_latency_ - this->total_util_task_duration_;

      // Calc and print the CPU percentage.  I add 0.5 to round to the
      // nearest integer before casting it to int.
      ACE_DEBUG ((LM_DEBUG,
                  "\t%% ORB Client CPU utilization: %u %%\n"
                  "\t%% Idle time: %u %%\n\n",
                  (u_int) (this->total_latency_high_ * 100 / this->total_latency_ + 0.5),
                  (u_int) (this->total_util_task_duration_ * 100 / this->total_latency_ + 0.5)));

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) UTILIZATION task performed \t%u computations\n"
                  "(%t) CLIENT task performed \t\t%u CORBA calls\n"
                  "(%t) Utilization test time is \t\t%f seconds\n"
                  "\t Ratio of computations to CORBA calls is %u.%u:1\n\n",
                  this->util_thread_->get_number_of_computations (),
                  this->ts_->loop_count_,
                  this->ts_->util_test_time_,
                  this->util_thread_->get_number_of_computations () / this->ts_->loop_count_,
                  (this->util_thread_->get_number_of_computations () % this->ts_->loop_count_)
                  * 100 / this->ts_->loop_count_));

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) utilization computation time is %A usecs\n",
                  this->util_task_duration_));
    }
}

void
Client_i::print_priority_inversion_stats (void)
{
  this->print_context_stats ();
  this->print_latency_stats ();
  this->print_util_stats ();
}

int
Client_i::start_servant (Task_State *ts,
                         ACE_Thread_Manager &thread_manager)
{
  char high_thread_args[BUFSIZ];

  static char hostname[BUFSIZ];

  if (ACE_OS::hostname (hostname, BUFSIZ) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "hostname"),
                       -1);
  ACE_OS::sprintf (high_thread_args,
                   "-ORBport %d "
                   "-ORBhost %s "
                   "-ORBobjrefstyle URL "
                   "-ORBsndsock 32768 "
                   "-ORBrcvsock 32768 ",
                   ACE_DEFAULT_SERVER_PORT,
                   hostname);

  Cubit_Task *high_priority_task;

  ACE_NEW_RETURN (high_priority_task,
                  Cubit_Task ((const char *) high_thread_args,
                              (const char *) "internet",
                              (u_int) 1,
                              &thread_manager,
                              (u_int) 0), // task id 0.
                  -1);

  this->high_priority_ =
    this->priority_.get_high_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant 0 with high priority %d\n",
              this->high_priority_));

  // Make the high priority task an active object.
   if (high_priority_task->activate (THR_BOUND | ACE_SCHED_FIFO,
                                     1,
                                     0,
                                     this->high_priority_) == -1)
     ACE_ERROR ((LM_ERROR,
                 "(%P|%t) %p\n"
                 "\thigh_priority_task->activate failed"));

   ACE_DEBUG ((LM_DEBUG,
               "(%t) Waiting for argument parsing\n"));

   ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ready_mon, GLOBALS::instance ()->ready_mtx_,-1));

   while (!GLOBALS::instance ()->ready_)
     GLOBALS::instance ()->ready_cnd_.wait ();

   ACE_DEBUG ((LM_DEBUG,
               "(%t) Argument parsing waiting done\n"));

   GLOBALS::instance ()->barrier_->wait ();

   this->ts_->one_ior_ = high_priority_task->get_servant_ior (0);
   return 0;
}

int
Client_i::do_priority_inversion_test (ACE_Thread_Manager *thread_manager)
{
  this->timer_.start ();
#if defined (VXWORKS)
  ctx = 0;
  ACE_NEW_RETURN (this->task_id_,
                  char[TASK_ID_LEN],
                  -1);
#endif /* VXWORKS */
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) <<<<<<< starting test on %D\n"));
  GLOBALS::instance ()->num_of_objs = 1;
  ACE_Thread_Manager server_thread_manager;
  GLOBALS::instance ()->use_name_service = 0;

  for (u_int j = 0; j < this->ts_->argc_; j++)
    if (ACE_OS::strcmp (this->ts_->argv_[j], "-u") == 0)
      {
        start_servant (this->ts_,
                       server_thread_manager);
        break;
      }

  // Create the clients.
  int result = this->activate_high_client (thread_manager);

  if (result < 0)
    return result;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Waiting for argument parsing\n"));

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ready_mon, this->ts_->ready_mtx_,-1));

  // wait on the condition variable until the high priority thread wakes us up.
  while (!this->ts_->ready_)
    this->ts_->ready_cnd_.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Argument parsing waiting done\n"));
  result = this->activate_low_client (thread_manager);

  if (result < 0)
    return result;

  result = this->activate_util_thread ();

  if (result < 0)
    return result;

  // Wait for all the client threads to be initialized before going
  // any further.
  this->ts_->barrier_->wait ();

  STOP_QUANTIFY;
  CLEAR_QUANTIFY;

  // collect the context switch data.
  this->get_context_switches ();

  // Wait for all the client threads to exit (except the utilization
  // thread).
  thread_manager->wait ();
  STOP_QUANTIFY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) >>>>>>> ending test on %D\n"));

  this->timer_.stop ();
  this->timer_.elapsed_time (this->delta_);

  // Signal the utilization thread to finish with its work..  only
  // if utilization test was specified.  See description of this
  // variable in header file.
  if (this->ts_->use_utilization_test_ == 1)
    {
      this->util_thread_->done_ = 1;

      // This will wait for the utilization thread to finish.
      this->util_thread_manager_.wait ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "-------------------------- Stats -------------------------------\n"));
  
  this->print_priority_inversion_stats ();
  return 0;
}

int
Client_i::do_thread_per_rate_test (ACE_Thread_Manager *thread_manager)
{
  Client CB_20Hz_client (thread_manager,
                         this->ts_,
                         CB_20HZ_CONSUMER);
  Client CB_10Hz_client (thread_manager,
                         this->ts_,
                         CB_10HZ_CONSUMER);
  Client CB_5Hz_client (thread_manager,
                        this->ts_,
                        CB_5HZ_CONSUMER);
  Client CB_1Hz_client (thread_manager,
                        this->ts_,
                        CB_1HZ_CONSUMER);
  ACE_Sched_Priority priority;

#if defined (VXWORKS)
  priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  priority = ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                             ACE_SCOPE_THREAD);
#else  /* ! VXWORKS */
  priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* ! ACE_WIN32 */
  ACE_DEBUG ((LM_DEBUG,
              "Creating 20 Hz client with priority %d\n",
              priority));

  if (CB_20Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                               1,
                               1,
                               priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) errno = %p: activate failed\n"));
  // The high priority thread is parsing the arguments, so wait on the
  // condition variable until it wakes us up.

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Waiting for argument parsing\n"));

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ready_mon, this->ts_->ready_mtx_,-1));

  // wait on the condition variable until the high priority thread wakes us up.
  while (!this->ts_->ready_)
    this->ts_->ready_cnd_.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Argument parsing waiting done\n"));

  priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_THREAD);
  ACE_DEBUG ((LM_DEBUG,
              "Creating 10 Hz client with priority %d\n",
              priority));

  if (CB_10Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                               1,
                               1,
                               priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) errno = %p: activate failed\n"));

  priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_THREAD);
  ACE_DEBUG ((LM_DEBUG,
              "Creating 5 Hz client with priority %d\n",
              priority));

  if (CB_5Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                              1,
                              1,
                              priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) errno = %p: activate failed\n"));

  priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                  priority,
                                                  ACE_SCOPE_THREAD);
  ACE_DEBUG ((LM_DEBUG,
              "Creating 1 Hz client with priority %d\n",
              priority));

  if (CB_1Hz_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                              1,
                              1,
                              priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) errno = %p: activate failed\n"));

  // Wait for all the threads to exit.
  thread_manager->wait ();

  ACE_DEBUG ((LM_DEBUG,
              "Test done.\n"
              "20Hz client latency : %A usec, jitter: %A usec\n"
              "10Hz client latency : %A usec, jitter: %A usec\n"
              "5Hz client latency : %A usec, jitter: %A usec\n"
              "1Hz client latency : %A usec, jitter: %A usec\n",
              CB_20Hz_client.get_latency (0), 
              CB_20Hz_client.get_jitter (0),
              CB_10Hz_client.get_latency (1),
              CB_10Hz_client.get_jitter (1),
              CB_5Hz_client.get_latency (2),
              CB_5Hz_client.get_jitter (2),
              CB_1Hz_client.get_latency (3),
              CB_1Hz_client.get_jitter (3) ));
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
#endif /* VXWORKS */
  int result;
  Client_i client;

  result = client.init (argc,argv);

  if (result < 0)
    return result;
  
  // Run the tests.
  client.run ();

#if defined (CHORUS)
  if (pccTimer (PCC2_TIMER1_STOP,
                &pTime) != K_OK)
    ACE_DEBUG ((LM_DEBUG,
                "pccTimer has a pending bench mark\n"));
#elif defined (VXWORKS)
  // Shoot myself.  Otherwise, there's a General Protection Fault.
  // This will leak memory, but that's preferable.  It looks like the
  // problem might be due to static objects in libTAO or liborbsvcs?
  int status;
  ACE_OS::thr_exit (&status);
#endif /* CHORUS */
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Globals,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Globals,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
