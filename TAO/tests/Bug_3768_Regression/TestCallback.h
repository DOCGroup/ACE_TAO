#ifndef TESTCALLBACK_H
#define TESTCALLBACK_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include "ace/Task.h"

class Worker : public ACE_Task_Base
{
public:
  Worker (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override
  {
    orb_->run ();
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
  TestCallback (TestCallback const &) = delete;
  TestCallback (TestCallback &&) = delete;
  TestCallback &operator= (TestCallback const &) = delete;
  TestCallback &operator= (TestCallback &&) = delete;

  // = The skeleton methods
  void start_threads_and_block () override;
  void test_call () override;
  void shutdown () override;

private:
  Worker work_;
};

#include /**/ "ace/post.h"
#endif /* TESTCALLBACK_H */
