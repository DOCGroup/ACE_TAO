// -*- C++ -*-

//=============================================================================
/**
 *  @file     FT_ClientORBInitializer.h
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================
#ifndef TAO_FT_CLIENTORB_INITIALIZER_H
#define TAO_FT_CLIENTORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "orbsvcs/FaultTolerance/FT_ClientORB_export.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// FT ORB initializer.
class TAO_FT_ClientORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual ::CORBA::LocalObject
{
public:
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:
  /// Register FTCORBA policy factories.
  void register_policy_factories (
         PortableInterceptor::ORBInitInfo_ptr info);

  /// Register the necessary interceptors.
  void register_client_request_interceptors (
         PortableInterceptor::ORBInitInfo_ptr info);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_FT_ORB_INITIALIZER_H */
