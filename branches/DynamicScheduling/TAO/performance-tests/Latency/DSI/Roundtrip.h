//
// $Id$
//

#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include "ace/pre.h"

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/corba.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual PortableServer::DynamicImplementation
  , public virtual PortableServer::RefCountServantBase
{
public:
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The DSI methods
  virtual void invoke (CORBA::ServerRequest_ptr request
                       ACE_ENV_ARG_DECL);
  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa
      ACE_ENV_ARG_DECL);

private:
  /// Use an ORB reference to conver strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ROUNDTRIP_H */
