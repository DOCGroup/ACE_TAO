// -*- C++ -*-
//
// $Id$

#ifndef TAO_CLIENT_INTERCEPTOR_H
#define TAO_CLIENT_INTERCEPTOR_H

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Echo_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Client_Request_Interceptor (const char *orb_id);
  // ctor.

  virtual ~Echo_Client_Request_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;

  CORBA::String_var orb_id_;
  // The ID of the ORB where this interceptor was created, usually
  // obtained from the ORBInitInfo

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CLIENT_INTERCEPTOR_H */
