#ifndef SERVERINTERCEPTOR_H
#define SERVERINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/ServerRequestInterceptorA.h"
#include "tao/PI/ORBInitInfo.h"

class ServerInterceptor
: public PortableInterceptor::ServerRequestInterceptor
{
 public:
  ServerInterceptor (ServerInterceptor const &) = delete;
  ServerInterceptor (ServerInterceptor &&) = delete;
  ServerInterceptor &operator= (ServerInterceptor const &) = delete;
  ServerInterceptor &operator= (ServerInterceptor &&) = delete;

  ServerInterceptor (IOP::CodecFactory_var);

  ~ServerInterceptor () override = default;

  char * name () override;

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr ri) override;

 private:
  char const *myname_;
  IOP::Codec_var codec;
};

#endif
