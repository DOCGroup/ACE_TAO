// $Id$

#include "tao/ClientRequestInterceptor_List.h"

#if !defined (__ACE_INLINE__)
# include "tao/ClientRequestInterceptor_List.inl"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           ClientRequestInterceptor_List,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

TAO_ClientRequestInterceptor_List::TAO_ClientRequestInterceptor_List (void)
  : interceptors_ ()
{
}

TAO_ClientRequestInterceptor_List::~TAO_ClientRequestInterceptor_List (void)
{
}

size_t
TAO_ClientRequestInterceptor_List::length (void)
{
  return this->interceptors_.size ();
}

void
TAO_ClientRequestInterceptor_List::length (size_t len)
{
  this->interceptors_.size (len);
}


PortableInterceptor::Interceptor_ptr
TAO_ClientRequestInterceptor_List::interceptor (size_t index)
{
  return this->interceptors_[index].in ();
}

void
TAO_ClientRequestInterceptor_List::add_interceptor (
  PortableInterceptor::ClientRequestInterceptor_ptr interceptor
  ACE_ENV_ARG_DECL)
{
  const size_t index = this->add_interceptor_i (interceptor
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->interceptors_[index] =
    PortableInterceptor::ClientRequestInterceptor::_duplicate (interceptor);
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

