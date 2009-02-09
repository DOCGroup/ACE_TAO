// $Id$

#ifndef SERVERINTERCEPTOR_H
#define SERVERINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/ServerRequestInterceptorA.h"
#include "tao/PI/ORBInitInfo.h"

class ServerInterceptor
: public PortableInterceptor::ServerRequestInterceptor
{
 public:
  ServerInterceptor (IOP::CodecFactory_var);

  ~ServerInterceptor ();

  virtual char * name ();

  virtual void destroy ();

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
                                                 PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri);

 private:
  const char *myname_;
  IOP::Codec_var codec;
};

#endif
