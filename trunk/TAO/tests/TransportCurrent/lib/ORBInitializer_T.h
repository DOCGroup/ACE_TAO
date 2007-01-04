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
  // A template class for ORBInitializer, which registers
  // interceptors. Partial specializations exist to handle client and
  // server request interceptors.
  template <typename I>
  class Current_Test_Export ORBInitializer : public PortableInterceptor::ORBInitializer
  {
  public:
    ORBInitializer (typename I::_ptr_type interceptor);
    ~ORBInitializer (void);

    virtual void pre_init(PortableInterceptor::ORBInitInfo*
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

    virtual void post_init(PortableInterceptor::ORBInitInfo*
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  private:
    typename I::_var_type interceptor_;
  };

}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ORBInitializer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ORBInitializer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif  /* ORBINITIALIZER_T_H */


