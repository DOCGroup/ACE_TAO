//
// $Id$
//

#ifndef COLLOCATED_SERVER_TASK_H
#define COLLOCATED_SERVER_TASK_H
#include /**/ "ace/pre.h"

#include "ace/Task.h"
#include "ace/Manual_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "TestC.h"

/// Implement a Task to run the server in a single thread
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (CORBA::ORB_ptr sorb,
               ACE_Manual_Event &cond,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc ();

  //Test::Roundtrip_prt get_reference ();
  Test::Roundtrip * get_reference ();

private:

  ACE_Manual_Event &cond_;
  /// Manual event to wake up the main thread to create a client
  /// thread.

  CORBA::ORB_var sorb_;
  Test::Roundtrip_var rt_var_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_SERVER_TASK_H */
