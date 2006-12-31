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


#ifndef ORBINITIALIZER_T_CPP
#define ORBINITIALIZER_T_CPP

#include "ORBInitializer_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Test
{

  template <typename I>
  ORBInitializer<I>::ORBInitializer (typename I::_ptr_type interceptor)
    : interceptor_ (I::_duplicate (interceptor))
  {
  }

  template <typename I>
  ORBInitializer<I>::~ORBInitializer (void)
  {
  }

  template <typename I>
  void
  ORBInitializer<I>::pre_init(PortableInterceptor::ORBInitInfo*
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
  }

}
#endif /* ORBINITIALIZER_T_CPP */
