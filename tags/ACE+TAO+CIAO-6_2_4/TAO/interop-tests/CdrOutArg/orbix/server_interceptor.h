// -*- C++ -*-
//
// $Id$

#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include <omg/PortableInterceptor.hh>

class Echo_Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Server_Request_Interceptor (void);
  // cotr.

  ~Echo_Server_Request_Interceptor ();
  // dotr.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

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
