// -*- C++ -*-

//=============================================================================
/**
 *  @file FTRT_ClientORB_Interceptor.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FTRT_CLIENTORB_INTERCEPTOR_H
#define TAO_FTRT_CLIENTORB_INTERCEPTOR_H

#include /**/ "ace/pre.h"


#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class FTRT_ClientORB_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:
  /// ctor.
  FTRT_ClientORB_Interceptor (CORBA::Long transaction_depth);

  /// dtor.
  virtual ~FTRT_ClientORB_Interceptor ();

  /// Canonical name of the interceptor.
  virtual char * name (void);

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);
private:
  const char *myname_;
  CORBA::Long transaction_depth_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif
