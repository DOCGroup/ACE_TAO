// $Id$

//**************************************************************************
//
// NAME :   client.C 
// DESCRIPTION:  
//
// Client for the Cubit example
//
//****************************************************************************

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "cubit.h"
#include "Task_Client.h"

ACE_RCSID(client, client, "$Id$")

//
// Mainline
//
int
main (int argc, char *argv[])
{
  Task_State ts (argc, argv);

  ACE::set_handle_limit ();

  Client low_client (&ts);

  // First activate the high priority client.

  Client high_client (&ts);

  ACE_Sched_Priority priority = 
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG, "Creating client with high priority\n"));

  if (high_client.activate (THR_BOUND, 1, 0, priority) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));

  priority = 
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  ACE_DEBUG ((LM_DEBUG, "Creating %d clients with low priority\n", ts.thread_count_ - 1));

  if (low_client.activate (THR_BOUND, ts.thread_count_ - 1, 0, priority) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));


  ACE_Thread_Manager::instance ()->wait ();

  ACE_OS::printf ("Test done.\n"
                  "High priority client latency : %f msec\n"
                  "Low priority client latency : %f msec\n",
                  high_client.get_high_priority_latency (),
                  low_client.get_low_priority_latency ());

  return 0;
}
