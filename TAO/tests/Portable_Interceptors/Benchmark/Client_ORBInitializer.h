// -*- C++ -*-

#ifndef TAO_CLIENT_ORB_INITIALIZER_H
#define TAO_CLIENT_ORB_INITIALIZER_H
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"
#include "Interceptor_Type.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Client ORB initializer.
class Client_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  /// Constructor
  explicit Client_ORBInitializer (Interceptor_Type interceptor_type);

  Client_ORBInitializer (Client_ORBInitializer const &) = delete;
  Client_ORBInitializer (Client_ORBInitializer &&) = delete;
  Client_ORBInitializer &operator= (Client_ORBInitializer const &) = delete;
  Client_ORBInitializer &operator= (Client_ORBInitializer &&) = delete;

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:
  Interceptor_Type const interceptor_type_;
  // The type of interceptor that this initializer will create
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_CLIENT_ORB_INITIALIZER_H */
