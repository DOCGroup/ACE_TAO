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
  
  virtual char * name ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy ()
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
                                                 PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

 private:
  const char *myname_;
  IOP::Codec_var codec;
};

#endif
