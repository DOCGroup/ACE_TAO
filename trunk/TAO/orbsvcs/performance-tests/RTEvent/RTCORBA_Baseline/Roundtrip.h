//
// $Id$
//

#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include "ace/pre.h"

#include "TestS.h"

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual POA_Test::Roundtrip
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual Test::Timestamp test_method (Test::Timestamp send_time
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include "ace/post.h"
#endif /* ROUNDTRIP_H */
