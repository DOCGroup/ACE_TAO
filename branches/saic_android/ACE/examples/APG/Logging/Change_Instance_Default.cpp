/**
 * $Id$
 *
 * Sample code from The ACE Programmer's Guide,
 * Copyright 2003 Addison-Wesley. All Rights Reserved.
 *
 * This code shows how to set the ACE_Log_Msg per-instance default
 * differently for groups of spawned threads.
 */

#include "ace/Log_Msg.h"
#include "ace/Thread_Manager.h"

ACE_THR_FUNC_RETURN worker (void *)
{
  // do some work
  return 0;
}

ACE_THR_FUNC_RETURN service (void *)
{
  // run the service
  return 0;
}

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  // Listing 1 code/ch03
  ACE_LOG_MSG->priority_mask (0, ACE_Log_Msg::PROCESS);
  ACE_Log_Msg::enable_debug_messages ();
  ACE_Thread_Manager::instance ()->spawn (service);
  ACE_Log_Msg::disable_debug_messages ();
  ACE_Thread_Manager::instance ()->spawn_n (3, worker);
  // Listing 1
  ACE_Thread_Manager::instance ()->wait ();
  return 0;
}
