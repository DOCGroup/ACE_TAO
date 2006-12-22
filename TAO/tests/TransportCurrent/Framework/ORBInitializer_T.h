// -*- C++ -*-

//=============================================================================
/**
 * @file ORBInitializer_T.h
 *
 * $Id$
 *
 * @author Iliyan jeliazkov <iliyan@ociweb.com>
 */
//=============================================================================

#ifndef ORBINITIALIZER_T_H
#define ORBINITIALIZER_T_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"

namespace Test
{
  // A template class for ORBInitializer, which registers
  // interceptors. Partial specializations exist to handle client and
  // server request interceptors.
  template <typename Interceptor>
  class ORBInitializer : public PortableInterceptor::ORBInitializer
  {
  public:
    ORBInitializer (typename Interceptor::_ptr_type interceptor)
      : interceptor_ (Interceptor::_duplicate (interceptor))
    {
    };

    ~ORBInitializer (void)
    {
    };

    virtual void pre_init(PortableInterceptor::ORBInitInfo* ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException))
    {
    };

    virtual void post_init(PortableInterceptor::ORBInitInfo* ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    typename Interceptor::_var_type interceptor_;
  };

}

#endif  /* ORBINITIALIZER_T_H */


