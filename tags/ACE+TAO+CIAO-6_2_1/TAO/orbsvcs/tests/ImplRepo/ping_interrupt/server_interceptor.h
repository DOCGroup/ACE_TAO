// -*- C++ -*-
//
// $Id$

#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PortableServer/POAC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Ping_Death_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Ping_Death_Request_Interceptor (int *counter);
  // cotr.

  ~Ping_Death_Request_Interceptor ();
  // dotr.
  void set_poa (PortableServer::POA_ptr poa);

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
  int *counter_;
  PortableServer::POA_var poa_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SERVER_INTERCEPTOR_H */
