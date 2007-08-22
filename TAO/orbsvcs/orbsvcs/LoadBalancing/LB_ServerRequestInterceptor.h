// -*- C++ -*-

//=============================================================================
/**
 * @file    LB_ServerRequestInterceptor.h
 *
 * $Id$
 *
 * @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_SERVER_REQUEST_INTERCEPTOR_H
#define TAO_LB_SERVER_REQUEST_INTERCEPTOR_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#include "orbsvcs/LoadBalancing/LoadBalancing_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LB_LoadAlert;

/**
 * @class TAO_LB_ServerRequestInterceptor
 *
 * @brief ServerRequestInterceptor that interacts with the TAO-shipped
 *        LoadAlert implementation.
 *
 * This ServerRequestInterceptor is responsible for redirecting
 * requests back to the LoadManager.
 */
class TAO_LoadBalancing_Export TAO_LB_ServerRequestInterceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_LB_ServerRequestInterceptor (TAO_LB_LoadAlert & load_alert);

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

protected:

  /// Destructor.
  /**
   * Protected destructor to enforce correct memory management via
   * reference counting.
   */
  ~TAO_LB_ServerRequestInterceptor (void);

private:

  TAO_LB_LoadAlert & load_alert_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif  /* TAO_LB_SERVER_REQUEST_INTERCEPTOR_H */
