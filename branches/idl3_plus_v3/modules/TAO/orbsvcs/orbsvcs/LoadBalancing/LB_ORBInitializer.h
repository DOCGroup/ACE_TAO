// -*- C++ -*-

//=============================================================================
/**
 *  @file LB_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_ORB_INITIALIZER_H
#define TAO_LB_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/Versioned_Namespace.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LB_ORBInitializer
 *
 * @brief ORBInitializer for the LB_Component Service_Object.
 *
 * This ORBInitializer simply creates and registers with the ORB the
 * IORInterceptor that handles transparent object group member
 * registration with the LoadManager, and registration of the
 * LoadAlert object necessary for load shedding.
 *
 * @par
 *
 * It also creates and registers the ServerRequestInterceptor that
 * facilitates load shedding.
 */
class TAO_LB_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  TAO_LB_ORBInitializer (const CORBA::StringSeq & object_groups,
                         const CORBA::StringSeq & repository_ids,
                         const char * location);

  /**
   * @name PortableInterceptor::ORBInitializer Methods
   *
   * Methods required by the PortableInterceptor::ORBInitializer
   * interface.
   */
  //@{
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
  //@}

private:

  /// List of stringified object group references.
  const CORBA::StringSeq object_groups_;

  /// List of RepositoryIds for object that will be load
  /// managed/balanced.
  const CORBA::StringSeq repository_ids_;

  /// Location at which the LoadBalancing component resides.
  CORBA::String_var location_;

  /// The CosLoadBalancing::LoadAlert servant to be used at this
  /// location.
  /**
   * This is the servant supplied by TAO's Load Balancer.  It is
   * used out of convencience.
   */
  TAO_LB_LoadAlert load_alert_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_LB_ORB_INITIALIZER_H */
