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
    public virtual ::CORBA::LocalObject
{
 public:
  Server_IORInterceptor_ORBInitializer () = default;
  Server_IORInterceptor_ORBInitializer (Server_IORInterceptor_ORBInitializer const &) = delete;
  Server_IORInterceptor_ORBInitializer (Server_IORInterceptor_ORBInitializer &&) = delete;
  Server_IORInterceptor_ORBInitializer &operator= (Server_IORInterceptor_ORBInitializer const &) = delete;
  Server_IORInterceptor_ORBInitializer &operator= (Server_IORInterceptor_ORBInitializer &&) = delete;

  //@{
  /// The pre-initialization hook.
  void pre_init (PortableInterceptor::ORBInitInfo_ptr info) override;

  /// The post-initialization hook.
  void post_init (PortableInterceptor::ORBInitInfo_ptr info) override;
  //@}
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* SERVER_IOR_INTERCEPTOR_ORB_INITIALIZER_H */
