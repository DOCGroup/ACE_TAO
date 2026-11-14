
#ifndef SERVER_LEAKS_PING_SERVICE_H
#define SERVER_LEAKS_PING_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Ping_Service interface
class Ping_Service
  : public virtual POA_Test::Ping_Service
{
public:
  explicit Ping_Service (CORBA::ORB_ptr orb);
  Ping_Service (Ping_Service const &) = delete;
  Ping_Service (Ping_Service &&) = delete;
  Ping_Service &operator= (Ping_Service const &) = delete;
  Ping_Service &operator= (Ping_Service &&) = delete;

  // = The skeleton methods
  void ping () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var const orb_;
};

#include /**/ "ace/post.h"
#endif /* SERVER_LEAKS_PING_SERVICE_H */
