// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_MemberLocator.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_MEMBER_LOCATOR_H
#define TAO_LB_MEMBER_LOCATOR_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class TAO_LB_LoadManager;

/**
 * @class TAO_LB_MemberLocator
 *
 * @brief ServantLocator implementation for the Load Balancer.
 *
 * This is a Servant Locator implementation that forwards requests to
 * a member returned by the Load Balancer.
 */
class TAO_LB_MemberLocator
  : public virtual PortableServer::ServantLocator,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor
  TAO_LB_MemberLocator (TAO_LB_LoadManager * load_balancer);

  /**
   * @name The PortableServer::ServantLocator methods.
   *
   * Methods required by the PortableServer::ServantLocator
   * interface.
   */
  //@{

  /// Clients requests are forwarded via the
  /// PortableServer::ForwardRequest exception thrown in this method.
  virtual PortableServer::Servant preinvoke (
      const PortableServer::ObjectId & oid,
      PortableServer::POA_ptr adapter,
      const char * operation,
      PortableServer::ServantLocator::Cookie & the_cookie);

  /// This method is no-op in this ServantLocator implementation.
  virtual void postinvoke (
      const PortableServer::ObjectId &oid,
      PortableServer::POA_ptr adapter,
      const char * operation,
      PortableServer::ServantLocator::Cookie the_cookie,
      PortableServer::Servant the_servant);

  //@}

private:

  /// The load balancer/manager implementation.
  TAO_LB_LoadManager * load_manager_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LB_MEMBER_LOCATOR_H */
