#ifndef TAO_UTILS_AUTO_FUNCTOR_CPP
#define TAO_UTILS_AUTO_FUNCTOR_CPP

#include "Auto_Functor.h"

#if !defined(__ACE_INLINE__)
# include "Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (Utils,
           Auto_Functor,
           "$Id$")

template<typename X, typename Functor>
TAO::Utils::Auto_Functor<X,Functor>::~Auto_Functor()
  ACE_THROW_SPEC (())
{
  reset(0);
}

template<typename X, typename Functor> void
TAO::Utils::Auto_Functor<X,Functor>::reset (X * p)
  ACE_THROW_SPEC (())
{
  if(p_ != 0)
  {
    f_(p_);
  }
  p_ = p;
}

template<typename X, typename Functor>void
TAO::Utils::Auto_Functor<X,Functor>::reset (X * p, Functor f)
  ACE_THROW_SPEC (())
{
  reset(p);
  f_ = f;
}

#endif /*TAO_UTILS_AUTO_FUNCTOR_CPP*/
