// -*- C++ -*-
//
// $Id$

#if TAO_HAS_INTERCEPTORS == 1
ACE_INLINE TAO_ClientRequestInterceptor_List::TYPE &
TAO_ClientRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}

// ------------------

ACE_INLINE TAO_ServerRequestInterceptor_List::TYPE &
TAO_ServerRequestInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

// ------------------

ACE_INLINE size_t
TAO_IORInterceptor_List::add_interceptor_helper (
    PortableInterceptor::Interceptor_ptr interceptor
    ACE_ENV_ARG_DECL
  )
{
  return this->add_interceptor_i (interceptor
                                  ACE_ENV_ARG_PARAMETER);
}

ACE_INLINE TAO_IORInterceptor_List::TYPE &
TAO_IORInterceptor_List::interceptors (void)
{
  return this->interceptors_;
}

