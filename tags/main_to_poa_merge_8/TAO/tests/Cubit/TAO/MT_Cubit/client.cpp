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

// Sumedh, can you please rename this file to be "clnt" or something
// so that it is consistent with the IDL_Cubit name scheme.  Or,
// conversely, rename the IDL Cubit files "client" and "server."

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "cubitC.h"
#include "Task_Client.h"
#include "Util_Thread.h"

// Sumedh, please move this stuff into a separate "initialization"
// file.
#if defined (VME_DRIVER)
#include <hostLib.h>
extern "C" STATUS vmeDrv (void);
extern "C" STATUS vmeDevCreate (char *);
#endif /* defined (VME_DRIVER) */

// This is the main routine of the client, where we create a high
// priority and a low priority client. we then activate the clients
// with the appropriate priority threads, and wait for them to
// finish. After they aer done, we compute the latency and jitter
// metrics and print them.

// Sumedh, can you please talk with David Levine to figure out how to
// make this main() function prototype cleaner?  For instance, take a
// look at how we deal with this stuff in the $ACE_ROOT/tests/*.cpp
// examples for VxWorks.

int
#if defined (VXWORKS) && defined (FORCE_ARGS)
main (int arg_c, char *arg_v [])
#else
main (int argc, char *argv [])
#endif /* defined (VXWORKS) && defined (FORCE_ARGS) */
{
  // Sumedh, the following code should be factored into a special
  // "initialization" function, rather than cluttering the main
  // program.
#if defined (VXWORKS)
    hostAdd ("mv2604d", "130.38.183.132");
#if defined (VME_DRIVER)
    STATUS status = vmeDrv ();
    if (status != OK)
      printf ("ERROR on call to vmeDrv()\n");
    status = vmeDevCreate ("/vme");
    if (status != OK)
      printf ("ERROR on call to vmeDevCreate()\n");
#endif /* defined (VME_DRIVER) */
#if defined (FORCE_ARGS)
    int argc = 7;
    char *argv[] = {"main",
                    "-d",
                    "3",   // Data Type
                    "-t",
                    "10",   // Thread Count
                    "-h",
                    "mv2604d"};  // Host name
#endif   /* defined (FORCE_ARGS) */
#endif   /* defined (VXWORKS) */

    // Sumedh, this main() function is *way* too long.  Please split
    // it up into multiple smaller functions.

  Task_State ts (argc, argv);

  // Make sure we've got plenty of socket handles.  This call will use
  // the default maximum.
  ACE::set_handle_limit ();

  // Sumedh, can you please fix all this stuff so that it's not
  // conditionally compiled, but instead is selected by a command-line
  // switch?
#if defined (PRIORITY_INV_TEST)
  // Create the clients
  Client high_priority_client (&ts);
  Client low_priority_client (&ts);
  
  // Create the daemon thread in its own <ACE_Thread_Manager>.
  // Sumedh, is there a reason why this is allocated dynamically?  In
  // particular, it is never deleted!  Can you just use the
  // ACE_Thread_Manager::instance () singleton or do you really need
  // your own copy?  If you need your own copy, can you consider
  // allocating it on the stack so you don't have to delete it (or
  // forget to delete it?).
  ACE_Thread_Manager *thr_mgr;

  ACE_NEW_RETURN (thr_mgr, 
                  ACE_Thread_Manager,
                  -1);

  Util_Thread util_thread (&ts, thr_mgr);

  ACE_Sched_Priority priority = 
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, 
                                    ACE_SCOPE_THREAD);
  
  // First activate the Utilization thread.  It will wait until all
  // threads have finished binding.
  util_thread.activate (THR_BOUND,
                        1,
                        0,
                        priority);


  // Now activate the high priority client.

  // @@ Sumedh, can you please document why we need this VxWorks
  // #ifdef?  Is there a way to make this more general so we don't
  // need the #ifdef?  In other words, please check with David or
  // Brian about this.

#if !defined (VXWORKS)
  priority = 
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, 
                                    ACE_SCOPE_THREAD);
#else
  ACE_Sched_Priority priority = 60;
#endif /* !defined (VXWORKS) */

  ACE_DEBUG ((LM_DEBUG,
              "Creating client with high priority\n"));

  if (high_priority_client.activate (THR_BOUND | ACE_SCHED_FIFO,
                                     1,
                                     0,
                                     priority) == -1)

    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate failed"));

  // @@ Sumedh, can you please document why we need this VxWorks
  // #ifdef?  Is there a way to make this more general so we don't
  // need the #ifdef?  In other words, please check with David or
  // Brian about this.

#if !defined (VXWORKS)
  priority = 
    ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO, 
                                         priority,
                                         ACE_SCOPE_THREAD);
#else
  priority = 65;
#endif /* !defined (VXWORKS) */

  ACE_DEBUG ((LM_DEBUG,
              "Creating %d clients with low priority\n", 
              ts.thread_count_ - 1));

  for (u_int i = 0; i < ts.thread_count_ - 1; i++)
    {
      // The first thread starts at min + 1, since the minimum
      // priority thread is the utilization thread.
     
      // get the next higher priority
      priority = ACE_Sched_Params::previous_priority (ACE_SCHED_FIFO,
                                                      priority,
                                                      ACE_SCOPE_THREAD);
      if (low_priority_client.activate (THR_BOUND,
                                        1,
                                        1,
                                        priority) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate failed"));
    }

#else  // Thread per Rate test
  // First activate the high priority client.
  Client CB_40Hz_client (&ts);
  Client CB_20Hz_client (&ts);
  Client CB_10Hz_client (&ts);
  Client CB_5Hz_client (&ts);
  Client CB_1Hz_client (&ts);

  ACE_Sched_Priority priority = 
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, 
                                    ACE_SCOPE_THREAD);

  // VxWorks priority of 0 causes problems.
  priority = 10;
  ACE_DEBUG ((LM_DEBUG, "Creating 40 Hz client with priority %d\n", priority));
  if (CB_40Hz_client.activate (THR_BOUND, 1, 0, priority++) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));

  ACE_DEBUG ((LM_DEBUG, "Creating 20 Hz client with priority %d\n", priority));
  if (CB_20Hz_client.activate (THR_BOUND, 1, 0, priority++) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
  
  ACE_DEBUG ((LM_DEBUG, "Creating 10 Hz client with priority %d\n", priority));
  if (CB_10Hz_client.activate (THR_BOUND, 1, 0, priority++) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
  
  ACE_DEBUG ((LM_DEBUG, "Creating 5 Hz client with priority %d\n", priority));
   if (CB_5Hz_client.activate (THR_BOUND, 1, 0, priority++) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));

   ACE_DEBUG ((LM_DEBUG, "Creating 1 Hz client with priority %d\n", priority));
   if (CB_1Hz_client.activate (THR_BOUND, 1, 0, priority++) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));

#endif /* defined (PRIORITY_INV_TEST) */

  // Wait for all the threads to exit.
  ACE_Thread_Manager::instance ()->wait ();

#if defined (PRIORITY_INV_TEST)
#if !defined (VXWORKS)
  ACE_DEBUG ((LM_DEBUG, "Test done.\n"
              "High priority client latency : %f msec, jitter: %f msec\n"
              "Low priority client latency : %f msec, jitter: %f msec\n",
              high_priority_client.get_high_priority_latency (),
              high_priority_client.get_high_priority_jitter (),
              low_priority_client.get_low_priority_latency (),
              low_priority_client.get_low_priority_jitter ()));
#else
  ACE_OS::printf ("Test done.\n"
                  "High priority client latency : %d usec\n"
                  "Low priority client latency : %d usec\n",
                  high_client.get_high_priority_latency (),
                  low_client.get_low_priority_latency ());
#endif /* !defined (VXWORKS) */

#else
  ACE_OS::printf ("Test done.\n"
                  "40Hz client latency : %d usec\n"
                  "20Hz client latency : %d usec\n"
                  "10Hz client latency : %d usec\n"
                  "5Hz client latency : %d usec\n"
                  "1Hz client latency : %d usec\n",
                  CB_40Hz_client.get_latency (0),
                  CB_20Hz_client.get_latency (1),
                  CB_10Hz_client.get_latency (2),
                  CB_5Hz_client.get_latency (3),
                  CB_1Hz_client.get_latency (4));
#endif /* defined (PRIORITY_INV_TEST) */

  util_thread.done_ = 1;
  thr_mgr->wait ();
  cerr << "utilization task performed " 
       << util_thread.get_number_of_computations ()
       << " computations" 
       << endl;
  
  return 0;
}
