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


// Global options used to configure various parameters.
// static char hostname[BUFSIZ];
// static char *ior_file = 0;
// static int base_port = ACE_DEFAULT_SERVER_PORT;
// static u_int num_of_objs = 2;
// static u_int use_name_service = 1;
// static u_int thread_per_rate = 0;
// static u_int use_multiple_priority = 0;
// static u_int run_utilization_test = 0;

int
Server::initialize (int argc, char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

#if defined (VXWORKS)
  // @@ Naga, can you please factor this code into a separate file?!
#if defined (VME_DRIVER)
   STATUS status = vmeDrv ();

   if (status != OK)
     ACE_DEBUG ((LM_DEBUG,
                 "ERROR on call to vmeDrv()\n"));

   status = vmeDevCreate ("/vme");

   if (status != OK)
     ACE_DEBUG ((LM_DEBUG,
                 "ERROR on call to vmeDevCreate()\n"));
#endif /* defined (VME_DRIVER) */

#if defined (FORCE_ARGS)
   int argc = 4;
   char *argv[] = 
   {
     "server",
     "-s",
     "-f",
     "ior.txt" 
   };
#endif /* defined (FORCE_ARGS) */
#endif /* defined (VXWORKS) */

   // Make sure we've got plenty of socket handles.  This call will
   // use the default maximum.
   ACE::set_handle_limit ();
   return 0;
}

int
Server::start_servants (ACE_Thread_Manager *serv_thr_mgr)
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
      else if (ACE_OS::strcmp (this->argv_[i], "-t") == 0
               && i - 1 < this->argc_)
        GLOBALS::instance ()->num_of_objs =
          ACE_OS::atoi (this->argv_ [i + 1]);
    }
  // Create an array to hold pointers to the Cubit objects.
  CORBA::String *cubits;

  ACE_NEW_RETURN (cubits,
                  CORBA::String [GLOBALS::instance ()->num_of_objs],
                  -1);
  
  char orbport[BUFSIZ];
  
  ACE_OS::sprintf (orbport,
                   "-ORBport %d ",
                   GLOBALS::instance ()->base_port);

  char orbhost[BUFSIZ];

  ACE_OS::sprintf (orbhost,
                   "-ORBhost %s ",
                   GLOBALS::instance ()->hostname);

  char *high_second_argv[] = {orbport,
                              orbhost,
                              "-ORBobjrefstyle URL ",
                              "-ORBsndsock 32768 ",
                              "-ORBrcvsock 32768 ",
                              0};

  ACE_ARGV high_argv (this->argv_,high_second_argv);

  Cubit_Task *high_priority_task;

  ACE_NEW_RETURN (high_priority_task,
                  Cubit_Task (high_argv.buf (),
                              "internet",
                              1,
                              serv_thr_mgr,
                              0), //task id 0.
                  -1);

  // @@ Naga, here's another place where we write the same code again.
  // Please make sure that this gets factored out into a macro or an
  // inline function!
#if defined (VXWORKS)
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF;
#elif defined (ACE_WIN32)
  ACE_Sched_Priority priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);
#else
  // @@ Naga/Sergio, why is there a "25" here?  This seems like to
  // much of a "magic" number.  Can you make this more "abstract?"
  ACE_Sched_Priority priority = ACE_THR_PRI_FIFO_DEF + 25;
#endif /* VXWORKS */

  ACE_DEBUG ((LM_DEBUG,
              "Creating servant 0 with high priority %d\n",
              priority));

  // Make the high priority task an active object.
  if (high_priority_task->activate (THR_BOUND | ACE_SCHED_FIFO,
                                    1,
                                    0,
                                    priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%P|%t) %p\n"
                "\thigh_priority_task->activate failed"));

  ACE_MT (ACE_GUARD_RETURN (ACE_Thread_Mutex, ready_mon, GLOBALS::instance ()->ready_mtx_,-1));

  while (!GLOBALS::instance ()->ready_)
    GLOBALS::instance ()->ready_cnd_.wait ();

  // Create an array to hold pointers to the low priority tasks.
  Cubit_Task **low_priority_task;

  ACE_NEW_RETURN (low_priority_task,
                  Cubit_Task *[GLOBALS::instance ()->num_of_objs],
                  -1);

  u_int number_of_low_priority_servants = 0;
  u_int number_of_priorities = 0;
  u_int grain = 0;
  u_int counter = 0;
  u_int j;

  number_of_low_priority_servants =
    GLOBALS::instance ()->num_of_objs - 1;

  // Drop the priority
  if (GLOBALS::instance ()->thread_per_rate == 1 
      || GLOBALS::instance ()->use_multiple_priority == 1)
    {
      number_of_priorities = 0;

      for (ACE_Sched_Priority_Iterator priority_iterator (ACE_SCHED_FIFO,
                                                          ACE_SCOPE_THREAD);
           priority_iterator.more ();
          priority_iterator.next ())
        number_of_priorities ++;

      // 1 priority is exclusive for the high priority client.
      number_of_priorities --;

      // Drop the priority, so that the priority of clients will
      // increase with increasing client number.
      for (j = 0;
           j < number_of_low_priority_servants;
           j++)
        priority =
          ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                               priority,
                                               ACE_SCOPE_THREAD);

      // Granularity of the assignment of the priorities.  Some OSs
      // have fewer levels of priorities than we have threads in our
      // test, so with this mechanism we assign priorities to groups
      // of threads when there are more threads than priorities.
      grain = number_of_low_priority_servants / number_of_priorities;
      counter = 0;

      if (grain <= 0)
        grain = 1;
    }
  else
    priority =
      ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                           priority,
                                           ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d servants starting at priority %d\n",
              number_of_low_priority_servants,
              priority));

  // Create the low priority servants.
  ACE_ARGV *low_argv;

  for (i = number_of_low_priority_servants; i > 0; i--)
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
      ACE_NEW_RETURN (low_argv,
                      ACE_ARGV (this->argv_,
                                low_second_argv),
                      -1);

      ACE_NEW_RETURN (low_priority_task [i - 1],
                      Cubit_Task (low_argv->buf (),
				  "internet",
				  1,
				  serv_thr_mgr,
				  i),
                      -1);

      // Make the low priority task an active object.
      if (low_priority_task [i - 1]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                               1,
                                               0,
                                               priority) == -1)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) %p\n"
                    "\tlow_priority_task[i]->activate"));
      ACE_DEBUG ((LM_DEBUG,
                  "Created servant %d with priority %d\n",
                  i,
                  priority));

      // Use different priorities on thread per rate or multiple
      // priority.
      if (GLOBALS::instance ()->use_multiple_priority == 1 
          || GLOBALS::instance ()->thread_per_rate == 1)
        {
          counter = (counter + 1) % grain;
          if (counter == 0
              &&
               //Just so when we distribute the priorities among the
               //threads, we make sure we don't go overboard.
              number_of_priorities * grain > number_of_low_priority_servants - (i - 1))
            // Get the next higher priority.
            priority = ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                                        priority,
                                                        ACE_SCOPE_THREAD);
        }
    } /* end of for() */

  GLOBALS::instance ()->barrier_->wait ();

  // Write the ior's to a file so the client can read them.
  {
    cubits[0] = high_priority_task->get_servant_ior (0);

    for (j = 0;
         j < GLOBALS::instance ()->num_of_objs - 1;
         ++j)
      cubits[j + 1] = low_priority_task[j]->get_servant_ior (0);

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
            ACE_OS::fprintf (ior_f, "%s\n", cubits[j]);
            ACE_DEBUG ((LM_DEBUG,
                        "cubits[%d] ior = %s\n",
                        j,
                        cubits[j]));
          }
      }

    if (ior_f != 0)
      ACE_OS::fclose (ior_f);
  }
  return 0;

}

// main routine.

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
#if defined (ACE_HAS_THREADS)
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
#if defined (__Lynx__)
          30,
#elif defined (VXWORKS) /* ! __Lynx__ */
          6,
#elif defined (ACE_WIN32)
  ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                  ACE_SCOPE_THREAD),
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

  if (server.initialize (argc, argv) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Initialization\n"),
                      1);

  // Create the daemon thread in its own <ACE_Thread_Manager>.
  ACE_Thread_Manager servant_thread_manager;

#if defined (NO_ACE_QUANTIFY)
  quantify_stop_recording_data();
  quantify_clear_data ();
  quantify_start_recording_data();
#endif /* NO_ACE_QUANTIFY */

  if (server.start_servants (&servant_thread_manager) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error creating the servants\n"),
                      1);

  ACE_DEBUG ((LM_DEBUG,
              "Wait for all the threads to exit\n"));

  // Wait for all the threads to exit.
  servant_thread_manager.wait ();
  //  ACE_Thread_Manager::instance ()->wait ();

#if defined (NO_ACE_QUANTIFY)
  quantify_stop_recording_data();
#endif /* NO_ACE_QUANTIFY */

#else
  ACE_DEBUG ((LM_DEBUG,
              "Test not run.  This platform doesn't seem to have threads.\n"));
#endif /* ACE_HAS_THREADS */
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Globals,ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Globals,ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
