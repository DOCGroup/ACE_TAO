// -*- C++ -*-

//=============================================================================
/**
 * @file LB_RPMS_Monitor_ORBInitializer.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_LB_RPMS_MONITOR_ORB_INITIALIZER_H
#define TAO_LB_RPMS_MONITOR_ORB_INITIALIZER_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/// Server side ORB initializer.
class TAO_LB_RPMS_Monitor_OrbInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  /**
   * @name The PortableInterceptor::ORBInitializer methods.
   */
  //@{

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  //@}

};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_LB_RPMS_MONITOR_ORB_INITIALIZER_H */
