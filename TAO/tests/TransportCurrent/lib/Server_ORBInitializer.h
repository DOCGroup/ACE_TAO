// -*- C++ -*-

//=============================================================================
/**
 * @file Server_ORBInitializer.h
 *
 * $Id$
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
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    Server_ORBInitializer (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
    virtual ~Server_ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*);

    virtual void post_init(PortableInterceptor::ORBInitInfo*);

  private:
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
  };

}

#include /**/ "ace/post.h"

#endif  /* SERVER_ORBINITIALIZER_T_H */

