// -*- C++ -*-

//=============================================================================
/**
 *  @file     FT_ORBInitializer.h
 *
 *  $Id$
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FT_ORB_INITIALIZER_H
#define TAO_FT_ORB_INITIALIZER_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "fault_tol_export.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// RTCORBA ORB initializer.
class TAO_FT_Export TAO_FT_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Register FTCORBA policy factories.
  void register_policy_factories (
         PortableInterceptor::ORBInitInfo_ptr info
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Register the necessary interceptors.
  void register_server_request_interceptors (
         PortableInterceptor::ORBInitInfo_ptr info
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void register_client_request_interceptors (
         PortableInterceptor::ORBInitInfo_ptr info
         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_FT_ORB_INITIALIZER_H */
