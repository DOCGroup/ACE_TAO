#ifndef CLIENTINTERCEPTOR_H
#define CLIENTINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

class ClientInterceptor
: public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual ::CORBA::LocalObject
{
public:
  ClientInterceptor ();

  ~ClientInterceptor () override = default;

  char *name () override;

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  ClientInterceptor (ClientInterceptor const &) = delete;
  ClientInterceptor (ClientInterceptor &&) = delete;
  ClientInterceptor &operator= (ClientInterceptor const &) = delete;
  ClientInterceptor &operator= (ClientInterceptor &&) = delete;

private:
  char const *myname_;
};

#endif
