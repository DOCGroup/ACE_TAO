
#ifndef CRASHED_CALLBACK_SERVICE_H
#define CRASHED_CALLBACK_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"
#include <cstddef>

/// Implement the Test::Service interface
class Service
  : public virtual POA_Test::Service
{
public:
  /// Constructor
  Service () = default;

  /// Dump the results.
  /**
   * In case of failure this exposes any incomplete runs.
   */
  void dump_results ();

  // = The skeleton methods
  void run_test (Test::Crashed_Callback_ptr callback) override;

  Service (Service const&) = delete;
  Service (Service&&) = delete;
  Service& operator= (Service const&) = delete;
  Service& operator= (Service&&) = delete;

  CORBA::ORB_var orb_ {};

private:
  /// Call the are_you_there callback multiple times.
  /**
   * @return Number of exceptions raised
   */
  std::size_t call_are_you_there (Test::Crashed_Callback_ptr callback);

  /// Call the test_oneway method mutiple times.
  /**
   * @return Number of exceptions raised
   */
  std::size_t call_test_oneway (Test::Crashed_Callback_ptr callback);

private:
  /// Count the number of tests executed
  std::size_t test_count_ {};
};

#include /**/ "ace/post.h"
#endif /* CRASHED_CALLBACK_SERVICE_H */
