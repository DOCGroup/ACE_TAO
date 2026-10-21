// -*- C++ -*-

//=============================================================================
//=============================================================================
#ifndef TAO249_SERVER_ORBINITIALIZER2_H
#define TAO249_SERVER_ORBINITIALIZER2_H

#include /**/ "ace/pre.h"

#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server_includeC.h"
#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

/// RTCORBA ORB initializer.
class Server_ORBInitializer2 :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  Server_ORBInitializer2 () = default;
  Server_ORBInitializer2 (Server_ORBInitializer2 const &) = delete;
  Server_ORBInitializer2 (Server_ORBInitializer2 &&) = delete;
  Server_ORBInitializer2 &operator= (Server_ORBInitializer2 const &) = delete;
  Server_ORBInitializer2 &operator= (Server_ORBInitializer2 &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;

private:
  /// Register the necessary interceptors.
  void register_server_request_interceptors (
         PortableInterceptor::ORBInitInfo_ptr info);
};

#include /**/ "ace/post.h"

#endif /* TAO249_SERVER_ORBINITIALIZER2_H */

