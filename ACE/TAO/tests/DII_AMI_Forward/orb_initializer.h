// -*- C++ -*-
//
// $Id$
//

#ifndef TAO_SERVER_ORB_INITIALIZER_H
#define TAO_SERVER_ORB_INITIALIZER_H
#include /**/ "ace/pre.h"

#include "tao/LocalObject.h"
#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class ForwardTest_Request_Interceptor;

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Server ORB initializer.
class Server_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer
  , public virtual CORBA::LocalObject
{
public:
  /// Constructor
  Server_ORBInitializer (void);

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);

  /// Return the created server request interceptor.  Only valid after
  /// post_init(), i.e. ORB_init(), has been called.
  ForwardTest_Request_Interceptor * server_interceptor (void);

private:
  /// Pointer to the server request interceptor.  ORB is responsible
  /// for storage.
  ForwardTest_Request_Interceptor * server_interceptor_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_SERVER_ORB_INITIALIZER_H */
