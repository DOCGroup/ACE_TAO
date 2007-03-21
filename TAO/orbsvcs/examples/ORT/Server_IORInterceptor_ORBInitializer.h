// $Id$

#ifndef SERVER_IOR_INTERCEPTOR_ORB_INITIALIZER_H
#define SERVER_IOR_INTERCEPTOR_ORB_INITIALIZER_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Server_IORInterceptor_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
 public:

  //@{
  /// The pre-initialization hook.
  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info);

  /// The post-initialization hook.
  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info);
  //@}

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_IOR_INTERCEPTOR_ORB_INITIALIZER_H */
