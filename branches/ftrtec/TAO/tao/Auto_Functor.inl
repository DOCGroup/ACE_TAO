// $Id$

template<typename X, typename Functor> ACE_INLINE
TAO::Utils::Auto_Functor_Ref<X,Functor>::
Auto_Functor_Ref(X * p, Functor f)
  : p_(p)
  , f_(f)
{
}

template<typename X, typename Functor> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::Auto_Functor(X * p, Functor f)
  ACE_THROW_SPEC (())
  : p_(p)
  , f_(f)
{
}

template<typename X, typename Functor> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::Auto_Functor(Auto_Functor & rhs)
  ACE_THROW_SPEC (())
  : p_(rhs.release())
  , f_(rhs.f_)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>&
TAO::Utils::Auto_Functor<X,Functor>:: operator=(Auto_Functor & rhs)
  ACE_THROW_SPEC (())
{
  reset(rhs.release());
  f_ = rhs.f_;
  return *this;
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
template<typename X, typename Functor> template<typename Y> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::Auto_Functor(Auto_Functor<Y,Functor>& rhs)
  ACE_THROW_SPEC (())
  : p_(rhs.release())
  , f_(rhs.f_)
{
}

template<typename X, typename Functor> template<typename Y>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>&
TAO::Utils::Auto_Functor<X,Functor>::operator=(Auto_Functor<Y,Functor>& rhs)
  ACE_THROW_SPEC (())
{
  reset(rhs.release());
  return *this;
}
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

template<typename X, typename Functor> ACE_INLINE X &
TAO::Utils::Auto_Functor<X,Functor>::operator*() const
  ACE_THROW_SPEC (())
{
  return *p_;
}

template<typename X, typename Functor>
ACE_INLINE X *
TAO::Utils::Auto_Functor<X,Functor>::operator->() const
  ACE_THROW_SPEC (())
{
  return p_;
}

template<typename X, typename Functor>
ACE_INLINE X *
TAO::Utils::Auto_Functor<X,Functor>::get()
  ACE_THROW_SPEC (())
{
  return p_;
}

template<typename X, typename Functor>
ACE_INLINE X *
TAO::Utils::Auto_Functor<X,Functor>::release()
  ACE_THROW_SPEC (())
{
  X * tmp = p_;
  p_ = 0;
  return tmp;
}

template<typename X, typename Functor>
ACE_INLINE Functor const &
TAO::Utils::Auto_Functor<X,Functor>::functor() const
  ACE_THROW_SPEC (())
{
  return f_;
}

template<typename X, typename Functor> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::Auto_Functor(Auto_Functor_Ref<X,Functor> rhs)
  ACE_THROW_SPEC (())
  : p_(rhs.p_)
  , f_(rhs.f_)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor> &
TAO::Utils::Auto_Functor<X,Functor>::operator=(Auto_Functor_Ref<X,Functor> rhs)
  ACE_THROW_SPEC (())
{
  if(rhs.p_ != p_)
  {
    reset(rhs.p_);
    f_ = rhs.f_;
  }
  return *this;
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)

template<typename X, typename Functor> template<typename Y> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::operator TAO::Utils::Auto_Functor_Ref<Y,Functor>()
  ACE_THROW_SPEC (())
{
  return TAO::Utils::Auto_Functor_Ref<Y,Functor>(release(), f_);
}

template<typename X, typename Functor> template<typename Y> ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::operator TAO::Utils::Auto_Functor<Y,Functor>()
  ACE_THROW_SPEC (())
{
  return TAO::Utils::Auto_Functor<Y,Functor>(release(), f_);
}

#else

template<typename X, typename Functor>ACE_INLINE
TAO::Utils::Auto_Functor<X,Functor>::operator TAO::Utils::Auto_Functor_Ref<X,Functor>()
  ACE_THROW_SPEC (())
{
  return TAO::Utils::Auto_Functor_Ref<X,Functor>(release(), f_);
}

#endif /* ACE_LACKS_MEMBER_TEMPLATES */
