// $Id$

#ifndef SERVERINITIALIZER_H
#define SERVERINITIALIZER_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

class ServerInitializer :
public virtual PortableInterceptor::ORBInitializer,
public virtual CORBA::LocalObject
{
 public:
  ServerInitializer ();

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

 private:
  int interceptor_type_;
};

#endif
