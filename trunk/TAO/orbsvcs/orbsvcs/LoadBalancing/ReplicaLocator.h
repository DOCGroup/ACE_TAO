// -*- C++ -*-

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_LoadBalancing
//
// = FILENAME
//    ReplicaLocator.h
//
// = AUTHOR
//    Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_REPLICA_LOCATOR_H
#define TAO_REPLICA_LOCATOR_H

#include "ace/pre.h"

#include "orbsvcs/LoadBalancingS.h"
#include "LoadBalancing_export.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class TAO_LB_LoadBalancer;

class TAO_LoadBalancing_Export TAO_LB_ReplicaLocator
  : public virtual PortableServer::ServantLocator
{
  // = TITLE
  //    Class that provides request forwarding.

  // = DESCRIPTION
  //    This is a Servant Locator implementation that forwards
  //    requests to a replica returned by the Load Balancer.

public:
  TAO_LB_ReplicaLocator (TAO_LB_LoadBalancer *load_balancer);
  // Constructor

  // = The ServantLocator methods
  virtual PortableServer::Servant preinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie & the_cookie
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  virtual void postinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie the_cookie,
      PortableServer::Servant the_servant
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  TAO_LB_LoadBalancer *load_balancer_;
  // The load balancer implementation.
};

#include "ace/post.h"

#endif  /* TAO_REPLICA_LOCATOR_H */
