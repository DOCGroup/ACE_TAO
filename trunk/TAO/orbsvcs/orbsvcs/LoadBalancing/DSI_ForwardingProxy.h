// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    DSI_ForwardingProxy.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef DSI_FORWARDING_PROXY_H
#define DSI_FORWARDING_PROXY_H

#include "ace/pre.h"

#include "orbsvcs/LoadBalancingS.h"
#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class TAO_LB_LoadBalancer;

// @@ Ossama: we have to change the implementation to use a
// ServantLocator, that can actually raise the ForwardingRequest
// exception.

class TAO_LoadBalancing_Export TAO_LB_DSI_ForwardingProxy : public PortableServer::DynamicImplementation
{
  // = TITLE
  //    Class that provides request forwarding.

  // = DESCRIPTION
  //    Using the Dynamic Skeleton Interface, this class intercepts
  //    requests from a client, and throws a location forwarding
  //    exception that contains the location of the server to which the
  //    client should redirect its requests.

public:
 TAO_LB_DSI_ForwardingProxy (TAO_LB_LoadBalancer *load_balancer,
                             const char *interface_id);
  // Constructor that sets the interface repository ID this DSI object
  // is associated with.

  // = The DynamicImplementation methods

  virtual void invoke (CORBA::ServerRequest_ptr request,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  virtual CORBA::RepositoryId _primary_interface (
                                   const PortableServer::ObjectId &oid,
                                   PortableServer::POA_ptr poa,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC (());

private:
  TAO_LB_LoadBalancer *load_balancer_;
  // The load balancer implementation.

  CORBA::String_var interface_id_;
  // The interface repository ID of the target object.

};

#endif  /* DSI_FORWARDING_PROXY_H */
