
#ifndef ROUNDTRIP_H
#define ROUNDTRIP_H
#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/Dynamic_Implementation.h"
#include "tao/PortableServer/PortableServer.h"
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
  explicit Roundtrip (CORBA::ORB_ptr orb);
  Roundtrip (Roundtrip const &) = delete;
  Roundtrip (Roundtrip &&) = delete;
  Roundtrip &operator= (Roundtrip const &) = delete;
  Roundtrip &operator= (Roundtrip &&) = delete;

  // = The DSI methods
  void invoke (CORBA::ServerRequest_ptr request) override;
  CORBA::RepositoryId _primary_interface (
      PortableServer::ObjectId const &oid,
      PortableServer::POA_ptr poa) override;

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
