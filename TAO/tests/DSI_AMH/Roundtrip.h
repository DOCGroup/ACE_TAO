//
// $Id$
//

#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/DynamicInterface/AMH_DSI_Response_Handler.h"

#include "tao/ORB.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

/// Implement the Test::Roundtrip interface
class Roundtrip
  : public virtual PortableServer::DynamicImplementation
{
public:
  /// Constructor
  Roundtrip (CORBA::ORB_ptr orb);

  // = The DSI methods

  /// Turns around and calls invoke, but using AMH style handler
  virtual void _dispatch (TAO_ServerRequest &request,
                          TAO::Portable_Server::Servant_Upcall *context);

  void invoke (CORBA::ServerRequest_ptr request);

  virtual void invoke (CORBA::ServerRequest_ptr request,
                       TAO_AMH_DSI_Response_Handler_ptr rh);
  virtual CORBA::RepositoryId _primary_interface (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr poa);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* ROUNDTRIP_H */
