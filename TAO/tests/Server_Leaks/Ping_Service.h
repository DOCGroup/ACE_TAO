//
// $Id$
//

#ifndef SERVER_LEAKS_PING_SERVICE_H
#define SERVER_LEAKS_PING_SERVICE_H
#include /**/ "ace/pre.h"

#include "TestS.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Ping_Service interface
class Ping_Service
  : public virtual POA_Test::Ping_Service
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Ping_Service (CORBA::ORB_ptr orb);

  // = The skeleton methods
  virtual void ping (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* SERVER_LEAKS_PING_SERVICE_H */
