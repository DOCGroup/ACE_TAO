// -*- C++ -*-

//=============================================================================
/**
 *  @file ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef ORB_INITIALIZER_H
#define ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/LoadBalancing/LB_LoadAlert.h"

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


class ServerRequestInterceptor;

/**
 * @class ORBInitializer
 *
 * @brief ORBInitializer
 *
 * ORBInitializer
 */
class ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  ORBInitializer (void);

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

  /// Return a reference to the LoadAlert object.
  TAO_LB_LoadAlert & load_alert (void);

  ServerRequestInterceptor * interceptor (void) const;

private:

  /// The CosLoadBalancing::LoadAlert servant to be used at this
  /// location.
  /**
   * This is the servant supplied by TAO's Load Balancer.  It is
   * used out of convencience.
   */
  TAO_LB_LoadAlert load_alert_;

  ServerRequestInterceptor * interceptor_;

};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* ORB_INITIALIZER_H */
