// -*- C++ -*-

//=============================================================================
/**
 *  $Id$
 */
//=============================================================================
#ifndef TAO249_SERVER_ORBINITIALIZER_H
#define TAO249_SERVER_ORBINITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server_includeC.h"
#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

/// RTCORBA ORB initializer.
class Server_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

private:

  /// Register the necessary interceptors.
  void register_server_request_interceptors (
         PortableInterceptor::ORBInitInfo_ptr info);

};

#include /**/ "ace/post.h"

#endif /* TAO249_SERVER_ORBINITIALIZER_H */

