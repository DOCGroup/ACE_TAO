// -*- C++ -*-

//=============================================================================
/**
 * $Id$
 */
//=============================================================================

#ifndef TAO249_SERVER_REQUEST_INTERCEPTOR2_H
#define TAO249_SERVER_REQUEST_INTERCEPTOR2_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"
#include "tao/ORB.h"
#include "tao/PI/PI.h"
#include "tao/PI_Server/PI_Server.h"

/**
* $Id$
*/
class ServerRequest_Interceptor2
: public virtual PortableInterceptor::ServerRequestInterceptor,
  public virtual TAO_Local_RefCounted_Object
{
public:

/// Constructor.
ServerRequest_Interceptor2 (void);

/// Destructor.
~ServerRequest_Interceptor2 (void);

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

static CORBA::Boolean has_ft_request_sc_;

private:

CORBA::ORB_var orb_;
};

#endif  /* TAO249_SERVER_REQUEST_INTERCEPTOR2_H */

