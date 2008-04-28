// -*- C++ -*-

//=============================================================================
/**
 *  @file   Set_Update_Interceptor.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SET_UPDATE_INTERCEPTOR_H
#define TAO_SET_UPDATE_INTERCEPTOR_H

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "ace/Atomic_Op.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// This is to remove "inherits via dominance" warnings from MSVC.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Set_Update_Interceptor
 *
 * @brief Interceptors for adding service context to set_update() and
 *        oneway_set_update() methods.
 *
 */
class TAO_Set_Update_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
  public virtual ::CORBA::LocalObject
{
public:
  TAO_Set_Update_Interceptor ();
  // ctor.

  virtual ~TAO_Set_Update_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAO_SET_UPDATE_INTERCEPTOR_H
