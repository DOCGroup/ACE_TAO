// -*- C++ -*-

//=============================================================================
/**
 * @file    ServerRequestInterceptor.h
 *
 * $Id$
 *
 * Implementation header for the server request interceptor for the
 * PortableInterceptor::Current test.
 *
 * @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef SERVER_REQUEST_INTERCEPTOR_H
#define SERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"
#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class ServerRequestInterceptor
 *
 * @brief Simple concrete server request interceptor.
 *
 * This server request interceptor
 */
class ServerRequestInterceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  ServerRequestInterceptor (PortableInterceptor::SlotId id,
                            PortableInterceptor::Current_ptr pi_current);

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
  virtual char * name (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (void)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (
      PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (
      PortableInterceptor::ServerRequestInfo_ptr ri)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
  //@}

private:

  /// The PICurrent slot ID allocated to this application during ORB
  /// initialization.
  PortableInterceptor::SlotId slot_id_;

  /// Reference to the PICurrent object.
  PortableInterceptor::Current_var pi_current_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_REQUEST_INTERCEPTOR_H */
