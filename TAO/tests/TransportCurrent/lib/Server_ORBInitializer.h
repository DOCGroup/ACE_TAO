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
#include /**/ "ace/config-all.h"

#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"

namespace Test
{

  class Current_Test_Export Server_ORBInitializer : public PortableInterceptor::ORBInitializer
  {
  public:
    Server_ORBInitializer (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
    ~Server_ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void post_init(PortableInterceptor::ORBInitInfo*
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
  };

}

#include /**/ "ace/post.h"

#endif  /* SERVER_ORBINITIALIZER_T_H */


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
#include /**/ "ace/config-all.h"

#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"

namespace Test
{

  class Current_Test_Export Server_ORBInitializer : public PortableInterceptor::ORBInitializer
  {
  public:
    Server_ORBInitializer (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
    ~Server_ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void post_init(PortableInterceptor::ORBInitInfo*
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
  };

}

#include /**/ "ace/post.h"

#endif  /* SERVER_ORBINITIALIZER_T_H */


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
#include /**/ "ace/config-all.h"

#include /**/ "Current_Test_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"

namespace Test
{

  class Current_Test_Export Server_ORBInitializer : public PortableInterceptor::ORBInitializer
  {
  public:
    Server_ORBInitializer (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);
    ~Server_ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void post_init(PortableInterceptor::ORBInitInfo*
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    PortableInterceptor::ServerRequestInterceptor_var interceptor_;
  };

}

#include /**/ "ace/post.h"

#endif  /* SERVER_ORBINITIALIZER_T_H */


