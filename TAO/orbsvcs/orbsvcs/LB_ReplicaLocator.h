// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_ReplicaLocator.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_REPLICA_LOCATOR_H
#define TAO_LB_REPLICA_LOCATOR_H

#include "ace/pre.h"

#include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServerC.h"

// Forward declaration.
class TAO_LB_LoadBalancer;

/**
 * @class TAO_LB_ReplicaLocator
 *
 * @brief ServantLocator implementation for the Load Balancer.
 *
 * This is a Servant Locator implementation that forwards requests to
 * a replica returned by the Load Balancer.
 */
class TAO_LB_ReplicaLocator
  : public virtual PortableServer::ServantLocator
{
public:

  /// Constructor
  TAO_LB_ReplicaLocator (TAO_LB_LoadBalancer *load_balancer);

  /**
   * @name The PortableServer::ServantLocator methods.
   */
  //@{

  /// Clients requests are forwarded via the
  /// PortableServer::ForwardRequest exception thrown in this method.
  virtual PortableServer::Servant preinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie & the_cookie
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));

  /// This method is no-op in this ServantLocator implementation.
  virtual void postinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char *operation,
      PortableServer::ServantLocator::Cookie the_cookie,
      PortableServer::Servant the_servant
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

private:

  /// The load balancer implementation.
  TAO_LoadBalancing_ReplicationManager_i *load_balancer_;

};

#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_LOCATOR_H */
