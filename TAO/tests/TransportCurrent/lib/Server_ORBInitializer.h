// -*- C++ -*-

//=============================================================================
/**
 * @file Server_ORBInitializer.h
 *
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef SERVER_ORBINITIALIZER_T_H
#define SERVER_ORBINITIALIZER_T_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"

#include "Current_Test_Export.h"

namespace Test
{
  class Current_Test_Export Server_ORBInitializer :
    public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
  {
  public:
    explicit Server_ORBInitializer (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
    ~Server_ORBInitializer () override;

    Server_ORBInitializer (Server_ORBInitializer const &) = delete;
    Server_ORBInitializer (Server_ORBInitializer &&) = delete;
    Server_ORBInitializer &operator= (Server_ORBInitializer const &) = delete;
    Server_ORBInitializer &operator= (Server_ORBInitializer &&) = delete;

    void pre_init (PortableInterceptor::ORBInitInfo *) override;

    void post_init (PortableInterceptor::ORBInitInfo *) override;

  private:
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
  };
}

#include /**/ "ace/post.h"

#endif  /* SERVER_ORBINITIALIZER_T_H */

