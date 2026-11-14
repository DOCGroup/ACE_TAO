// -*- C++ -*-

//=============================================================================
//=============================================================================

#ifndef TAO249_SERVER_REQUEST_INTERCEPTOR2_H
#define TAO249_SERVER_REQUEST_INTERCEPTOR2_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"
#include "tao/TimeBaseC.h"

class TAO249_ServerRequest_Interceptor2
  : public virtual PortableInterceptor::ServerRequestInterceptor,
  public virtual ::CORBA::LocalObject
{
public:
  TAO249_ServerRequest_Interceptor2 () = default;

  TAO249_ServerRequest_Interceptor2 (TAO249_ServerRequest_Interceptor2 const &) = delete;
  TAO249_ServerRequest_Interceptor2 (TAO249_ServerRequest_Interceptor2 &&) = delete;
  TAO249_ServerRequest_Interceptor2 &operator= (TAO249_ServerRequest_Interceptor2 const &) = delete;
  TAO249_ServerRequest_Interceptor2 &operator= (TAO249_ServerRequest_Interceptor2 &&) = delete;

  ~TAO249_ServerRequest_Interceptor2 () override = default;

  /**
   * @name Methods Required by the Server Request Interceptor Interface
   *
   * These methods implement the pure virtual ServerRequestInterceptor
   * operations.
   */
  //@{
  /// Return the name of this server request interceptor.
  char * name () override;

  void destroy () override;

  void receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (
    PortableInterceptor::ServerRequestInfo_ptr ri) override;
  //@}

protected:
  static TimeBase::TimeT get_now ();

private:
  CORBA::Long client_id_ {};
};

#endif  /* TAO249_SERVER_REQUEST_INTERCEPTOR2_H */

