//
// $Id$
//

#ifndef COLLOCATED_TEST_CLIENT_TASK_H
#define COLLOCATED_TEST_CLIENT_TASK_H
#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/ORB.h"


/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

  /// Constructor
  Client_Task (const char *input,
               const char *simple_test_ior,
               CORBA::ORB_ptr corb,
               ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  int svc (void);
 
  CORBA::ULong error_count () const;
  
private:
  const char *input_;
  const char *simple_test_input_;

  CORBA::ORB_var corb_;
  CORBA::ULong error_count_;
};

#include /**/ "ace/post.h"
#endif /* COLLOCATED_TEST_CLIENT_TASK_H */
