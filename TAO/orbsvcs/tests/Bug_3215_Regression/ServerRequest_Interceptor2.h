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
#include <atomic>

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

  void set_server_iogr (CORBA::Object_ptr obj);

  /**
   * @name Methods Required by the Server Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all server request interceptors.
   */
  //@{
  /// Return the name of this ServerRequestinterceptor.
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

private:
  TimeBase::TimeT get_now () const;

  std::atomic<unsigned int> invocation_count_ {};
  std::atomic<bool> expired_ {};
  CORBA::Object_var server_iogr_;
};

#endif  /* TAO249_SERVER_REQUEST_INTERCEPTOR2_H */

