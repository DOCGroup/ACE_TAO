// -*- C++ -*-

//=============================================================================
/**
 * @file Server_ORBInitializer.h
 *
 * $Id$
 *
 * Implementation header for the PortableInterceptor::ForwardRequest
 * exception test server side ORB initializer.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_SERVER_ORB_INITIALIZER_H
#define TAO_SERVER_ORB_INITIALIZER_H

#include /**/ "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// Forward declaration.
class Server_Request_Interceptor;

/// Server side ORB initializer.
class Server_ORBInitializer :
  public virtual PortableInterceptor::ORBInitializer,
  public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  Server_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the created server request interceptor.  Only valid after
  /// post_init(), i.e. ORB_init(), has been called.
  PortableInterceptor::ServerRequestInterceptor_ptr server_interceptor (void);

private:

  /// Pointer to the server request interceptor.  ORB is responsible
  /// for storage.
  PortableInterceptor::ServerRequestInterceptor_var server_interceptor_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_ORB_INITIALIZER_H */
