// -*- C++ -*-
//
// $Id$

namespace TAO
{
  ACE_INLINE
  ClientRequestInterceptor_Adapter::ClientRequestInterceptor_Adapter (
    TAO_ClientRequestInterceptor_List::TYPE & interceptors,
    Invocation_Base * invocation)
    : interceptors_ (interceptors)
    , invocation_ (invocation)
    //    , invoke_status_
    , len_ (interceptors.size ())
    , stack_size_ (0)
    , info_ (0)
  {
  }
}
