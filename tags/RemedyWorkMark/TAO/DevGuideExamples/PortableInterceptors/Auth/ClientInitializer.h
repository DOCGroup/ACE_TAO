// $Id$

#ifndef CLIENTINITIALIZER_H
#define CLIENTINITIALIZER_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

class ClientInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual ::CORBA::LocalObject
{
public:
  ClientInitializer (void);
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);
  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
};

#endif
