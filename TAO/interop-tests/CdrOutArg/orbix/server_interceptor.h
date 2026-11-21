// -*- C++ -*-
#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include <omg/PortableInterceptor.hh>

/// Server-side echo interceptor.  For checking interceptor visually only.
class Echo_Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  Echo_Server_Request_Interceptor ();

  ~Echo_Server_Request_Interceptor ();

  /// Canonical name of the interceptor.
  virtual char * name ();

  virtual void destroy ();

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr);

private:
  const char *myname_;
};

#endif /* TAO_SERVER_INTERCEPTOR_H */
