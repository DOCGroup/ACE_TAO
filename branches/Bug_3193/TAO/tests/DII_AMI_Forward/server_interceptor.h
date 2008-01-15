// -*- C++ -*-
//
// $Id$

#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class ForwardTest_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor

{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  ForwardTest_Request_Interceptor (void);
  // cotr.

  ForwardTest_Request_Interceptor (CORBA::Object_ptr forward_location);

  ~ForwardTest_Request_Interceptor ();
  // dotr.

  void forward_reference (CORBA::Object_ptr forward_location);

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr);

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr ri);

  bool forward_location_done() const;
private:
  const char *myname_;
  CORBA::Object_ptr forward_location_;
  bool forward_location_done_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SERVER_INTERCEPTOR_H */
