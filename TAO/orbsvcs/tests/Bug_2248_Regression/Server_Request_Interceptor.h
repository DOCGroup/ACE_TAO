// -*- C++ -*-

//=============================================================================
/**
 * @file Server_Request_Interceptor.h
 */
//=============================================================================

#ifndef BUG_2248_REGRESSION_SERVER_REQUEST_INTERCEPTOR_H
#define BUG_2248_REGRESSION_SERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#include <atomic>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"
#include "testsC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Server_Request_Interceptor
 *
 * @brief Simple concrete server request interceptor.
 */
class Server_Request_Interceptor
  : public virtual Simple_ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  Server_Request_Interceptor () = default;

  Server_Request_Interceptor (Server_Request_Interceptor const &) = delete;
  Server_Request_Interceptor (Server_Request_Interceptor &&) = delete;
  Server_Request_Interceptor &operator= (Server_Request_Interceptor const &) = delete;
  Server_Request_Interceptor &operator= (Server_Request_Interceptor &&) = delete;

  ~Server_Request_Interceptor () override = default;

  /// Set the references to which requests will be forwarded.
  void forward_reference (char const * iorstr) override;

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
  /// IOR of the forwarding target.
  CORBA::String_var iorstr_;
  /// Prevent concurrent requests from forwarding more than once.
  std::atomic<bool> request_forwarded_ {};
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* BUG_2248_REGRESSION_SERVER_REQUEST_INTERCEPTOR_H */
