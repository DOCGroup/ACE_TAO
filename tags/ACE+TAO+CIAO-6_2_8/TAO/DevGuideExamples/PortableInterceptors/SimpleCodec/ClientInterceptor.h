// $Id$

#ifndef CLIENTINTERCEPTOR_H
#define CLIENTINTERCEPTOR_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/PI/ClientRequestInterceptorA.h"
#include "tao/PI/ORBInitInfo.h"

class ClientInterceptor:
  public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual ::CORBA::LocalObject
{

 public:
  ClientInterceptor (IOP::CodecFactory_var) ;

  virtual ~ClientInterceptor ();

  virtual char * name ();

  virtual void destroy ();

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

 private:
  const char *myname_;
  IOP::Codec_var codec;

};

#endif
