// -*- C++ -*-
//=============================================================================
/**
 *  @file     ClientRequest_Interceptor.h
 */
//=============================================================================
#ifndef CLIENTREQUEST_INTERCEPTOR_H
#define CLIENTREQUEST_INTERCEPTOR_H
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
* @class ClientRequest_Interceptor
*
*/
class ClientRequest_Interceptor
: public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual ::CORBA::LocalObject
{
public:
  ClientRequest_Interceptor ();
  ClientRequest_Interceptor (ClientRequest_Interceptor const &) = delete;
  ClientRequest_Interceptor (ClientRequest_Interceptor &&) = delete;
  ClientRequest_Interceptor &operator= (ClientRequest_Interceptor const &) = delete;
  ClientRequest_Interceptor &operator= (ClientRequest_Interceptor &&) = delete;

  ~ClientRequest_Interceptor () override = default;

  /// Canonical name of the interceptor.
  char *name () override;

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

static CORBA::Boolean success_flag_;

private:
/// Name of the interceptor
  char const *name_;
};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /*CLIENTREQUEST_INTERCEPTOR_H*/

