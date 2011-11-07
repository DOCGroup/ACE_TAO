//
// $Id$
//

#ifndef CALLBACK_SERVICE_H
#define CALLBACK_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Service interface
class Service
  : public virtual POA_Test::Service
{
public:
  /// Constructor
  Service (CORBA::ORB_ptr orb);

  /// Dump the results.
  /**
   * In case of failure this exposes any incomplete runs.
   */
  void dump_results (void);

  // = The skeleton methods
  virtual void run_test (Test::Callback_ptr callback);

private:
  /// Call the are_you_there callback multiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_are_you_there (Test::Callback_ptr callback);

  /// Call the test_oneway method mutiple times.
  /**
   * @return Number of exceptions raised
   */
  int call_test_oneway (Test::Callback_ptr callback);

private:
  /// Count the number of tests executed
  int test_count_;

  /// Use an ORB reference to shutdown the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* CALLBACK_SERVICE_H */
