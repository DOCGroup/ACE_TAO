// -*- C++ -*-
#ifndef TAO_CLIENT_INTERCEPTOR_H
#define TAO_CLIENT_INTERCEPTOR_H

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Client-side echo interceptor.  For checking interceptor visually only.
class Echo_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  Echo_Client_Request_Interceptor (int& result);

  // Canonical name of the interceptor.
  char * name () override;

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

protected:
  ~Echo_Client_Request_Interceptor () override = default;

private:
  int& result_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CLIENT_INTERCEPTOR_H */
