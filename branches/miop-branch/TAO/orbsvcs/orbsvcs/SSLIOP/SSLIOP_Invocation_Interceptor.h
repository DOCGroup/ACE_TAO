// -*- C++ -*-

// ===================================================================
/**
 *  @file   SSLIOP_Invocation_Interceptor.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SSLIOP_INVOCATION_INTERCEPTOR_H
#define TAO_SSLIOP_INVOCATION_INTERCEPTOR_H

#include "ace/pre.h"

#include "SSLIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/SSLIOPC.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_SSLIOP_Server_Invocation_Interceptor
 *
 * @brief Secure invocation server request interceptor.
 *
 * This server request interceptor rejects insecure request
 * invocations if the effective target object policy requires secure
 * invocations.
 */
class TAO_SSLIOP_Export TAO_SSLIOP_Server_Invocation_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  TAO_SSLIOP_Server_Invocation_Interceptor (SSLIOP::Current_ptr current,
                                            int no_protection);

  /**
   * @name PortableInterceptor::ServerRequestInterceptor Methods
   *
   * Methods required by the
   * PortableInterceptor::ServerRequestInterceptor interface.
   */
  //@{
  virtual char * name (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
  //@}

protected:

  /// Destructor
  /**
   * Protected destructor to force deallocation by the reference
   * counting mechanism.
   */
  ~TAO_SSLIOP_Server_Invocation_Interceptor (void);

private:

  /**
   * @name Copying and Assignment
   *
   * Protected to prevent copying through the copy constructor and the
   * assignment operator.
   */
  //@{
  ACE_UNIMPLEMENTED_FUNC (
    TAO_SSLIOP_Server_Invocation_Interceptor (const TAO_SSLIOP_Server_Invocation_Interceptor &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_SSLIOP_Server_Invocation_Interceptor &))
  //@}

private:

  /// Reference to the current SSLIOP execution context.
  SSLIOP::Current_var ssliop_current_;

  /// If set to 1, then the secure invocation server request
  /// interceptor will allow requests coming in on the insecure port
  /// to be handled.
  int no_protection_;

};

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_INVOCATION_INTERCEPTOR_H */
