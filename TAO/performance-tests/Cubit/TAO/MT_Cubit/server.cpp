// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Andy Gokhale,
//    Sumedh Mungee,
//    Sergio Flores-Gaitan, and 
//    Nagarajan Surendran
//
// ============================================================================

#include "server.h"
#include "ace/Sched_Params.h"

#if defined (NO_ACE_QUANTIFY)
#include "quantify.h"
#endif /* NO_ACE_QUANTIFY */

ACE_RCSID(MT_Cubit, server, "$Id$")

#if defined (VXWORKS) && defined (FORCE_ARGS)
char *force_argv[]=
{
  "server",
  "-s",
  "-f",
  "ior.txt" 
};
#endif /* defined (VXWORKS) && defined (FORCE_ARGS) */

Server::Server (void)
  :argc_ (0),
   argv_ (0),
   cubits_ (0),
   high_priority_task_ (0),
   low_priority_tasks_ (0),
   high_argv_ (0),
   low_argv_ (0)
{
}

int
Server::initialize (int argc, char **argv)
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

   return 0;
}

void
Server::prelim_args_process (void)
{
  int i;

  for (i = 0; i < this->argc_ ; i++)
    {
      if (ACE_OS::strcmp (this->argv_[i], "-p") == 0
          && i - 1 < this->argc_)
        {
          GLOBALS::instance ()->base_port =
            ACE_OS::atoi (this->argv_[i + 1]);
          ACE_DEBUG ((LM_DEBUG,
                      "base_port:%d",
                      GLOBALS::instance()->base_port));
        }
      else if (ACE_OS::strcmp (this->argv_[i], "-h") == 0
               && i - 1 < this->argc_)
        ACE_OS::strcpy (GLOBALS::instance ()->hostname,
                        this->argv_[i+1]);
    }
}

void
Server::init_low_priority (void)
{
  ACE_Sched_Priority prev_priority = this->high_priority_;

  // Drop the priority.
  if (GLOBALS::instance ()->thread_per_rate == 1 
      || GLOBALS::instance ()->use_multiple_priority == 1)
    this->low_priority_ =
      this->priority_.get_low_priority (this->num_low_priority_,
                                        prev_priority,
                                        1);
  else
    this->low_priority_ =
      this->priority_.get_low_priority (this->num_low_priority_,
                                        prev_priority,
                                        0);

  this->num_priorities_ = this->priority_.number_of_priorities ();
  this->grain_ = this->priority_.grain ();
  this->counter_ = 0;
}

// Write the ior's to a file so the client can read them.

int
Server::write_iors (void)
{
  u_int j;

  // By this time the num of objs should be set properly.
  ACE_NEW_RETURN (this->cubits_,
                  CORBA::String [GLOBALS::instance ()->num_of_objs],
                  -1);

  this->cubits_[0] = ACE_OS::strdup (this->high_priority_task_->get_servant_ior (0));

  for (j = 1;
       j < GLOBALS::instance ()->num_of_objs;
       ++j)
    this->cubits_[j] =
      ACE_OS::strdup (this->low_priority_tasks_[j-1]->get_servant_ior (0));

  FILE *ior_f = 0;

  if (GLOBALS::instance ()->ior_file != 0)
    ior_f = ACE_OS::fopen (GLOBALS::instance ()->ior_file,
                           "w");

  for (j = 0;
       j < GLOBALS::instance ()->num_of_objs;
       ++j)
    {
      if (ior_f != 0)
        {
          ACE_OS::fprintf (ior_f, "%s\n", this->cubits_[j]);
          ACE_DEBUG ((LM_DEBUG,
                      "this->cubits_[%d] ior = %s\n",
                      j,
                      this->cubits_[j]));
        }
    }

  if (ior_f != 0)
    ACE_OS::fclose (ior_f);

  return 0;
}

int
Server::activate_high_servant (ACE_Thread_Manager *serv_thr_mgr)
{
  char orbport[BUFSIZ];
  char orbhost[BUFSIZ];

  ACE_OS::sprintf (orbport,
                   "-ORBport %d ",
                   GLOBALS::instance ()->base_port);
  ACE_OS::sprintf (orbhost,
                   "-ORBhost %s ",
                   GLOBALS::instance ()->hostname);

  char *high_second_argv[] = {orbport,
                              orbhost,
                              "-ORBobjrefstyle URL ",
                              "-ORBsndsock 32768 ",
                              "-ORBrcvsock 32768 ",
                              0};

  ACE_NEW_RETURN (this->high_argv_,
                  ACE_ARGV (this->argv_,high_second_argv),
                  -1);

  ACE_NEW_RETURN (this->high_priority_task_,
                  Cubit_Task (this->high_argv_->buf (),
                              "internet",
                              1,
                              serv_thr_mgr,
                              0), //task id 0.
                  -1);

  // Make the high priority task an active object.
  if (this->high_priority_task_->activate 
      (THR_BOUND | ACE_SCHED_FIFO,
       1,
       0,
       this->high_priority_) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n"
                "\thigh_priority_task->activate failed"));

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, 
			    ready_mon, 
			    GLOBALS::instance ()->ready_mtx_,
			    -1));

  // Wait on the condition variable for the high priority client to
  // finish parsing the arguments.
  while (!GLOBALS::instance ()->ready_)
    GLOBALS::instance ()->ready_cnd_.wait ();

  // Default return success.
  return 0;
}

int
Server::activate_low_servants (ACE_Thread_Manager *serv_thr_mgr)
{
  char orbport[BUFSIZ];
  char orbhost[BUFSIZ];
  int i;

  ACE_OS::sprintf (orbhost,
                   "-ORBhost %s ",
                   GLOBALS::instance ()->hostname);

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d servants starting at priority %d\n",
              this->num_low_priority_,
              this->low_priority_));

  // Create the low priority servants.
  ACE_NEW_RETURN (this->low_priority_tasks_,
                  Cubit_Task *[GLOBALS::instance ()->num_of_objs],
                  -1);

  for (i = this->num_low_priority_;
       i > 0;
       i--)
    {
      ACE_OS::sprintf (orbport,
                      "-ORBport %d",
                       GLOBALS::instance ()->base_port == 0
                       ? (int) 0 
                       : GLOBALS::instance ()->base_port + i);

      char *low_second_argv[] = {orbport,
                                 orbhost,
                                 "-ORBobjrefstyle URL ",
                                 "-ORBsndsock 32768 ",
                                 "-ORBrcvsock 32768 ",
                                 0};
      ACE_NEW_RETURN (this->low_argv_,
                      ACE_ARGV (this->argv_,
                                low_second_argv),
                      -1);

      ACE_NEW_RETURN (this->low_priority_tasks_ [i - 1],
                      Cubit_Task (this->low_argv_->buf (),
				  "internet",
				  1,
				  serv_thr_mgr,
				  i),
                      -1);

      // Make the low priority task an active object.
      if (this->low_priority_tasks_ [i - 1]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                               1,
                                               0,
                                               this->low_priority_) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n"
                    "\tthis->low_priority_tasks_[i]->activate"));
      ACE_DEBUG ((LM_DEBUG,
                  "Created servant %d with priority %d\n",
                  i,
                  this->low_priority_));

      // Use different priorities on thread per rate or multiple
      // priority.
      if (GLOBALS::instance ()->use_multiple_priority == 1 
          || GLOBALS::instance ()->thread_per_rate == 1)
        {
          this->counter_ = (this->counter_ + 1) % this->grain_;

          if (this->counter_ == 0
               //Just so when we distribute the priorities among the
               //threads, we make sure we don't go overboard.
              && this->num_priorities_ * this->grain_ > this->num_low_priority_ - (i - 1))
            // Get the next higher priority.
            this->low_priority_ = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                                   this->low_priority_,
                                                                   ACE_SCOPE_THREAD);
        }
    } /* end of for() */

  // default return success.
  return 0;
}

int
Server::start_servants (ACE_Thread_Manager *serv_thr_mgr)
{
  int result;
  // Do the preliminary argument processing for options -p and -h.
  this->prelim_args_process ();

  // Find the priority for the high priority servant.
  this->high_priority_ = this->priority_.get_high_priority ();

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant 0 with high priority %d\n",
              this->high_priority_));

  // Activate the high priority servant task
  if (this->activate_high_servant (serv_thr_mgr) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failure in activating high priority servant\n"),
                      -1);

  this->num_low_priority_ =
    GLOBALS::instance ()->num_of_objs - 1;

  // Initialize the priority of the low priority servants.
  this->init_low_priority ();

  // Activate the low priority servants.
  if (this->activate_low_servants (serv_thr_mgr) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Failure in activating low priority servant\n"),
                      -1);
  // Wait in the barrier.
  GLOBALS::instance ()->barrier_->wait ();
  result = this->write_iors ();
  if (result != 0)
    return result;

  return 0;
}

// The main() function.

#if defined (VXWORKS)
  // Rename main to server to make it easier to run both client and
  // server on one target.
extern "C"
int
server (int argc, char *argv[])
{
  ACE_Object_Manager ace_object_manager;
#else
int
main (int argc, char *argv[])
{
#endif /* VXWORKS */
  // Dummy code to create the GLOBALS object in the global memory
  // instead of TSS.
  GLOBALS::instance ();

  Server server;

  if (server.initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);

  // Create the daemon thread in its own <ACE_Thread_Manager>.
  ACE_Thread_Manager servant_thread_manager;

  STOP_QUANTIFY;
  CLEAR_QUANTIFY;
  START_QUANTIFY;

  if (server.start_servants (&servant_thread_manager) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      1);
  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));
  // Wait for all the threads to exit.
  servant_thread_manager.wait ();
  //  ACE_Thread_Manager::instance ()->wait ();
  STOP_QUANTIFY;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Globals,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Globals,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
