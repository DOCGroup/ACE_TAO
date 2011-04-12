// -*- C++ -*-

//=============================================================================
/**
 * @file Server_Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * PortableInterceptor::ForwardRequest test.
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
#include "tao/PortableInterceptorC.h"
#include "tao/PI_Server/PI_Server.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Server_Request_Interceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * This server request interceptor forwards an initial client request
 * to the primary server since the client request interceptor should
 * have attempted to forward a request to the secondary server.  That
 * request will then be forwarded back to the primary server, at which
 * point the request will be handled.
 *
 * The PortableInterceptor::ForwardRequest code for the
 * receive_request_service_contexts() interception point (which is not
 * in the servant skeleton) is slightly different from the
 * corresponding code in the server skeleton.  Hence,
 * PortableInterceptor::ForwardRequest support is tested for both
 * cases by throwing that exception in the
 * receive_request_service_contexts() and the receive_request()
 * interception points.
 *
 * See the README file for test details.
 */
class Server_Request_Interceptor
  : public virtual ForwardRequestTest::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:

  /// Constructor.
  Server_Request_Interceptor (void);

  /// Set the references to which requests will be forwarded.
  virtual void forward_references (CORBA::Object_ptr obj1,
                                   CORBA::Object_ptr obj2);

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
  virtual char * name (void);

  virtual void destroy (void);

  virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_exception (
      PortableInterceptor::ServerRequestInfo_ptr ri);

  virtual void send_other (
      PortableInterceptor::ServerRequestInfo_ptr ri);
  //@}

protected:

  /// Destructor.
  ~Server_Request_Interceptor (void);

private:

  /// The number of requests intercepted by this interceptor.
  CORBA::ULong request_count_;

  /// References to the two objects used in this test.
  CORBA::Object_ptr obj_[2];

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
