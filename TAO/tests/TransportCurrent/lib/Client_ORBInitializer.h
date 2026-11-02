// -*- C++ -*-

//=============================================================================
/**
 * @file Client_ORBInitializer.h
 *
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef CLIENT_ORBINITIALIZER_T_H
#define CLIENT_ORBINITIALIZER_T_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config.h"

#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/PortableInterceptorC.h"

namespace Test
{
  class Current_Test_Export Client_ORBInitializer :
    public virtual PortableInterceptor::ORBInitializer,
    public virtual ::CORBA::LocalObject
  {
  public:
    explicit Client_ORBInitializer (PortableInterceptor::ClientRequestInterceptor_ptr interceptor);
    ~Client_ORBInitializer () override;

    Client_ORBInitializer (Client_ORBInitializer const &) = delete;
    Client_ORBInitializer (Client_ORBInitializer &&) = delete;
    Client_ORBInitializer &operator= (Client_ORBInitializer const &) = delete;
    Client_ORBInitializer &operator= (Client_ORBInitializer &&) = delete;

    void pre_init (PortableInterceptor::ORBInitInfo *) override;

    void post_init (PortableInterceptor::ORBInitInfo *) override;

  private:
    PortableInterceptor::ClientRequestInterceptor_var interceptor_;
  };
}

#include /**/ "ace/post.h"

#endif  /* CLIENT_ORBINITIALIZER_T_H */


