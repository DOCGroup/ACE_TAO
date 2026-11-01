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
  ClientInitializer (ClientInitializer const &) = delete;
  ClientInitializer (ClientInitializer &&) = delete;
  ClientInitializer &operator= (ClientInitializer const &) = delete;
  ClientInitializer &operator= (ClientInitializer &&) = delete;

  ClientInitializer () = default;
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;
  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
};

#endif

