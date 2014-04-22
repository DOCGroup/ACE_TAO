//
// $Id: TestCallback.h
//

#ifndef TESTCALLBACK_H
#define TESTCALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Task.h"

class Worker : public ACE_Task_Base
{
public:
  Worker(CORBA::ORB_ptr orb) : orb_(CORBA::ORB::_duplicate(orb))
  {
  }

  int svc(void)
  {
    orb_->run();
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

/// Implement the Test::TestCallback interface
class TestCallback
  : public virtual POA_Test::TestCallback
{
public:
  /// Constructor
  TestCallback (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void start_threads_and_block (void);
  virtual void test_call (void);
  virtual void shutdown (void);

private:
  Worker work_;
};

#include /**/ "ace/post.h"
#endif /* TESTCALLBACK_H */
