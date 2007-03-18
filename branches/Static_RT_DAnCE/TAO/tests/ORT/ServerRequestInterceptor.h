// -*- C++ -*-

//=============================================================================
/**
 * @file ServerRequestInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * ORT test.
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

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/CORBA_String.h"

#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/PI_Server.h"


#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class ServerRequestInterceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * This server request interceptor tests some of the ORT-specific
 * methods in the ServerRequestInfo object passed to the server
 * request interception points.
 */
class ServerRequestInterceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  ServerRequestInterceptor (const char * orb_id,
                            PortableServer::Current_ptr poa_current);

  /**
   * @name Methods Required by the Server Request Interceptor
   *       Interface
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

private:

  /// The ORBid of the ORB with which this interceptor is registered.
  CORBA::String_var orb_id_;

  /// Reference to the POACurrent object.
  /**
   * This reference will be used to verify that the POA that
   * dispatched the request intercepted by this interceptor was infact
   * a child POA, and not the RootPOA.
   */
  PortableServer::Current_var poa_current_;

};


#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
