// $Id$

#include "IORInterceptor_List.h"

#if !defined (__ACE_INLINE__)
# include "IORInterceptor_List.inl"
#endif /* ! __ACE_INLINE__ */


ACE_RCSID (IORInterceptor,
           Interceptor_List,
           "$Id$")


TAO_IORInterceptor_List::TAO_IORInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_IORInterceptor_List::~TAO_IORInterceptor_List (void)
{
}

size_t
TAO_IORInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_IORInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}

PortableInterceptor::Interceptor_ptr
TAO_IORInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index].in ();
}

void
TAO_IORInterceptor_List::add_interceptor (
  PortableInterceptor::IORInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  const size_t index = this->add_interceptor_i (interceptor
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::IORInterceptor::_duplicate (interceptor);
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<PortableInterceptor::IORInterceptor_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<PortableInterceptor::IORInterceptor_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
