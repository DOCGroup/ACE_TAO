// -*- C++ -*-
//
// $Id$

#ifndef COLLOCATED_SERVER_TASK_H
#define COLLOCATED_SERVER_TASK_H

#include /**/ "ace/pre.h"

#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

/// Implement a Task to run the server in a single thread
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (const ACE_TCHAR *output,
               CORBA::ORB_ptr sorb,
               ACE_Manual_Event &me,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);

private:
  /// Output file for IOR
  const ACE_TCHAR *output_;

  /// Manual event to wake up the main thread to create a client
  /// thread.
  ACE_Manual_Event &me_;

  CORBA::ORB_var sorb_;
};

#include /**/ "ace/post.h"

#endif  /* COLLOCATED_SERVER_TASK_H */
