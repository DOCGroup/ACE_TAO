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

#include "ace/pre.h"

#include "LB_LoadAlert.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


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
    public virtual TAO_Local_RefCounted_Object
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
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
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


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_LB_ORB_INITIALIZER_H */
