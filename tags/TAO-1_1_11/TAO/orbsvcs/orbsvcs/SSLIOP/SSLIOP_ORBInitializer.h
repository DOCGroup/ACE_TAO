// -*- C++ -*-

//=============================================================================
/**
 *  @file    SSLIOP_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SSLIOP_ORB_INITIALIZER_H
#define TAO_SSLIOP_ORB_INITIALIZER_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

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

/**
 * @name TAO_SSLIOP_ORBInitializer
 *
 * @brief
 * ORB initializer that registers all SSLIOP-specific interceptors and
 * object references.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_SSLIOP_ORBInitializer (int no_protection);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// If set to 1, then the secure invocation server request
  /// interceptor will be configured to allow requests coming in on
  /// the insecure port to be handled.
  int no_protection_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_SSLIOP_ORB_INITIALIZER_H */
