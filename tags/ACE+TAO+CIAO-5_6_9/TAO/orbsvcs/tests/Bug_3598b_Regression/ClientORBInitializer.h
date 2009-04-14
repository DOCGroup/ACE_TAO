// -*- C++ -*-

//=============================================================================
/**
 *  @file     ClientORBInitializer.h
 *
 *  $Id$
 */
//=============================================================================
#ifndef CLIENTORBINITIALIZER_H
#define CLIENTORBINITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PortableInterceptorC.h"
#include "ClientRequest_Interceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// RTCORBA ORB initializer.
class ClientORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* CLIENTORBINITIALIZER_H */
