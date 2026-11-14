
#ifndef SHUTDOWN_HELPER_H
#define SHUTDOWN_HELPER_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Shutdown_Helper interface
class Shutdown_Helper
  : public virtual POA_Test::Shutdown_Helper
{
public:
  explicit Shutdown_Helper (CORBA::ORB_ptr orb);
  Shutdown_Helper (Shutdown_Helper const &) = delete;
  Shutdown_Helper (Shutdown_Helper &&) = delete;
  Shutdown_Helper &operator= (Shutdown_Helper const &) = delete;
  Shutdown_Helper &operator= (Shutdown_Helper &&) = delete;

  // = The skeleton methods
  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* SHUTDOWN_HELPER_H */
