// -*- C++ -*-

//=============================================================================
/**
 * @file    ServerRequestInterceptor.h
 *
 * $Id$
 *
 * @author  Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================


#ifndef TAO_SERVER_REQUEST_INTERCEPTOR_H
#define TAO_SERVER_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "ace/Atomic_Op.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_LB_LoadAlert;
TAO_END_VERSIONED_NAMESPACE_DECL

/**
 * @class ServerRequestInterceptor
 *
 * @brief ServerRequestInterceptor that calculates the number of
 *        requests arriving per second.
 *
 * This ServerRequestInterceptor is responsible for redirecting
 * requests back to the LoadManager.
 */
class ServerRequestInterceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  ServerRequestInterceptor (void);

  /**
   * @name Methods Required by the ServerRequestInterceptor
   *       Interface
   *
   * These are the canonical methods required for all
   * ServerRequestInterceptors.
   */
  //@{
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

  CORBA::Long request_count (void);

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce correct memory management via
   * reference counting.
   */
  ~ServerRequestInterceptor (void);

private:

  /// The number of requests that have arrived on the server.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, long> request_count_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */


#endif  /* TAO_SERVER_REQUEST_INTERCEPTOR_H */
