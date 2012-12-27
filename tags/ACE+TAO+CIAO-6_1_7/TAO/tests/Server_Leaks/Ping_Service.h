//
// $Id$
//

#ifndef SERVER_LEAKS_PING_SERVICE_H
#define SERVER_LEAKS_PING_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Ping_Service interface
class Ping_Service
  : public virtual POA_Test::Ping_Service
{
public:
  /// Constructor
  Ping_Service (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void ping (void);

  virtual void shutdown (void);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* SERVER_LEAKS_PING_SERVICE_H */
