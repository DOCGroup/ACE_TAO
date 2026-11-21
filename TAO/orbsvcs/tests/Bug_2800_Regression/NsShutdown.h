#ifndef SHUTDOWN_H
#define SHUTDOWN_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::NsShutdown interface
class NsShutdown
  : public virtual POA_Test::NsShutdown
{
public:
  explicit NsShutdown (CORBA::ORB_ptr orb);

  NsShutdown (NsShutdown const &) = delete;
  NsShutdown (NsShutdown &&) = delete;
  NsShutdown &operator= (NsShutdown const &) = delete;
  NsShutdown &operator= (NsShutdown &&) = delete;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* SHUTDOWN_H */
