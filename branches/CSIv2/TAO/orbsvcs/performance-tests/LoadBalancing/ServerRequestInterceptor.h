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

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#include "ace/Atomic_Op.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_LB_LoadAlert;

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
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (
      PortableInterceptor::ServerRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
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
  ACE_Atomic_Op<ACE_Thread_Mutex, long> request_count_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */


#endif  /* TAO_SERVER_REQUEST_INTERCEPTOR_H */
