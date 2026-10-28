// -*- C++ -*-
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
  Echo_Client_Request_Interceptor ();
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor &&) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor &&) = delete;
  // ctor.

  ~Echo_Client_Request_Interceptor () override = default;
  // dtor.

  char *name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

private:
  char const *myname_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CLIENT_INTERCEPTOR_H */
