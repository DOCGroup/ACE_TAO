#ifndef SERVERINITIALIZER_H
#define SERVERINITIALIZER_H

#include "tao/PortableInterceptorC.h"
#include "tao/PI/PI.h"

class ServerInitializer :
public virtual PortableInterceptor::ORBInitializer
{
 public:
  ServerInitializer ();
  ServerInitializer (ServerInitializer const &) = delete;
  ServerInitializer (ServerInitializer &&) = delete;
  ServerInitializer &operator= (ServerInitializer const &) = delete;
  ServerInitializer &operator= (ServerInitializer &&) = delete;

  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;

 private:
  int interceptor_type_;
};

#endif
