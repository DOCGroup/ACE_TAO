// -*- C++ -*-

// $Id$

#include "DSI_ForwardingProxy.h"
#include "LoadBalancer_i.h"

ACE_RCSID(orbsvcs, DSI_ForwardingProxy, "$Id$")

DSI_ForwardingProxy::DSI_ForwardingProxy (LoadBalancer_Impl *lb,
                                          const char *id)
  : load_balancer_ (lb), // Hopefully these pointers won't be zero!
    interface_id_ (id)
{
  // @@ Ossama: why is this comment useful?

  // Nothing else
}

void
DSI_ForwardingProxy::invoke (CORBA::ServerRequest_ptr /* request */,
                             CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  CORBA::Object_ptr replica =
    this->load_balancer_->replica (ACE_TRY_ENV);
  ACE_CHECK;

  // Throw a forward exception to force the client to redirect its
  // requests to the Replica chosen by the LoadBalancer.

  // @@ We should NOT be throwing this exception inside user code.
  //    Instead we should be using servant locators.
  ACE_THROW (PortableServer::ForwardRequest (
               CORBA::Object::_duplicate (replica)));
}

CORBA::RepositoryId
DSI_ForwardingProxy::_primary_interface (
                         const PortableServer::ObjectId &,
                         PortableServer::POA_ptr,
                         CORBA::Environment &)
  ACE_THROW_SPEC (())
{
  return CORBA::string_dup (this->interface_id_.in ());
}
