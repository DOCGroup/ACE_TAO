
#ifndef CRASHED_CALLBACK_SERVICE_H
#define CRASHED_CALLBACK_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Service interface
class Service
  : public virtual POA_Test::Service
{
public:
  /// Constructor
  Service ();

  /// Dump the results.
  /**
   * In case of failure this exposes any incomplete runs.
   */
  void dump_results ();

  // = The skeleton methods
  virtual void run_test (Test::Crashed_Callback_ptr callback);

  CORBA::ORB_var orb_;

private:
  /// Call the are_you_there callback multiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_are_you_there (Test::Crashed_Callback_ptr callback);

  /// Call the test_oneway method mutiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_test_oneway (Test::Crashed_Callback_ptr callback);

private:
  /// Count the number of tests executed
  int test_count_;
};

#include /**/ "ace/post.h"
#endif /* CRASHED_CALLBACK_SERVICE_H */
