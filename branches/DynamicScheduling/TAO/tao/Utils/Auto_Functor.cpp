#ifndef Auto_Functor__cpp_
#define Auto_Functor__cpp_
/**
 * @file Auto_Functor.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */

#include "Auto_Functor.h"

#if !defined(__ACE_INLINE__)
# include "Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

template<typename X, typename Functor>
TAO::Utils::Auto_Functor<X,Functor>::
~Auto_Functor() throw()
{
  reset(0);
}

template<typename X, typename Functor>
void TAO::Utils::Auto_Functor<X,Functor>::
reset (X * p) throw()
{
  if(p_ != 0)
  {
    f_(p_);
  }
  p_ = p;
}

template<typename X, typename Functor>
void TAO::Utils::Auto_Functor<X,Functor>::
reset (X * p, Functor f) throw()
{
  reset(p);
  f_ = f;
}

#endif // Auto_Functor__cpp_
