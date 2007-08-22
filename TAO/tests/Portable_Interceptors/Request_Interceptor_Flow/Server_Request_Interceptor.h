// -*- C++ -*-

//=============================================================================
/**
 * @file Server_Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * request interceptor flow test.
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

#include "Request_Interceptor.h"

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Server_Request_Interceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * See the README file for test details.
 */
class Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object,
    public virtual Request_Interceptor
{
public:

  /// Constructor.  Accepts the name of the interceptor.
  Server_Request_Interceptor (const char *name);

  /**
   * @name Methods Required by the Server Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all server request interceptors.
   */
  //@{
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

private:

  /// Returns 1 if the server-side test is currently being run, and 0
  /// otherwise.
  CORBA::Boolean server_side_test (
    PortableInterceptor::ServerRequestInfo_ptr info);

private:

  /// Variable used to keep track of the scenario count.  Used to get
  /// around the inability to call RequestInfo::arguments() in the
  /// receive_service_contexts() interception point.
  CORBA::ULong scenario_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
