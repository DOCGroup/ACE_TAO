// -*- C++ -*-

//=============================================================================
/**
 * @file Server_Request_Interceptor.h
 *
 * Implementation header for the server request interceptor for the
 * IOGR manipulation  test
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVER_REQUEST_INTERCEPTOR_H
#define SERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "testC.h"
#include "tao/LocalObject.h"
#include <array>

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
  : public virtual ForwardRequestTest::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  Server_Request_Interceptor () = default;
  Server_Request_Interceptor (Server_Request_Interceptor const&) = delete;
  Server_Request_Interceptor (Server_Request_Interceptor&&) = delete;
  Server_Request_Interceptor& operator= (Server_Request_Interceptor const&) = delete;
  Server_Request_Interceptor& operator= (Server_Request_Interceptor&&) = delete;

  /// Destructor.
  ~Server_Request_Interceptor () override = default;

  /// Set the references to which requests will be forwarded.
  void forward_references (CORBA::Object_ptr obj1,
                           CORBA::Object_ptr obj2) override;

  /// Reset to initial state, like counter etc, keeps the object-references
  void reset () override;

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
  /// The number of requests intercepted by this interceptor.
  CORBA::ULong request_count_ {};

  /// References to the two objects used in this test.
  std::array<CORBA::Object_var, 2> obj_ {};

  /// Flag to indicate whether a forward request was thrown.
  bool forward_request_thrown_ {};
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
