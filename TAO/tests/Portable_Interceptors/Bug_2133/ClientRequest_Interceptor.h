// -*- C++ -*-
//=============================================================================
/**
 *  @file     ClientRequest_Interceptor.h
 *
 *  $Id$
 */
//=============================================================================
#ifndef CLIENTREQUEST_INTERCEPTOR_H
#define CLIENTREQUEST_INTERCEPTOR_H
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
* @class ClientRequest_Interceptor
*
*/
class ClientRequest_Interceptor
: public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual TAO_Local_RefCounted_Object
{
public:

ClientRequest_Interceptor (void);

~ClientRequest_Interceptor (void);

/// Canonical name of the interceptor.
virtual char * name ();


virtual void destroy ();

virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri);

virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

static CORBA::Boolean success_flag_;

private:

/// Name of the interceptor
const char *name_;

};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /*CLIENTREQUEST_INTERCEPTOR_H*/

