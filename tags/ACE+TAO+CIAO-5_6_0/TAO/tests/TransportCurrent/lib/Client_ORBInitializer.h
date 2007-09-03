// -*- C++ -*-

//=============================================================================
/**
 * @file Client_ORBInitializer.h
 *
 * $Id$
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
    public virtual TAO_Local_RefCounted_Object
  {
  public:
    Client_ORBInitializer (PortableInterceptor::ClientRequestInterceptor_ptr interceptor);
    virtual ~Client_ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*);

    virtual void post_init(PortableInterceptor::ORBInitInfo*);

  private:
    PortableInterceptor::ClientRequestInterceptor_var interceptor_;
  };

}

#include /**/ "ace/post.h"

#endif  /* CLIENT_ORBINITIALIZER_T_H */



