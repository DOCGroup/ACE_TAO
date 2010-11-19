// -*- C++ -*-

#ifndef TAO_CLIENT_ORB_INITIALIZER_H
#define TAO_CLIENT_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/LocalObject.h"

/// Client side ORB initializer.
class Client_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual CORBA::LocalObject
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENT_ORB_INITIALIZER_H */
