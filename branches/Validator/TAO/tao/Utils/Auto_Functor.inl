// $Id$

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor_Ref<X,Functor>::
Auto_Functor_Ref(X * p, Functor f)
  : p_(p)
  , f_(f)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
Auto_Functor(X * p, Functor f) throw()
  : p_(p)
  , f_(f)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
Auto_Functor(Auto_Functor & rhs) throw()
  : p_(rhs.release())
  , f_(rhs.f_)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>&
TAO::Utils::Auto_Functor<X,Functor>::
operator=(Auto_Functor & rhs) throw()
{
  reset(rhs.release());
  f_ = rhs.f_;
  return *this;
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
template<typename X, typename Functor> template<typename Y>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
Auto_Functor(Auto_Functor<Y,Functor>& rhs)
  throw()
  : p_(rhs.release())
  , f_(rhs.f_)
{
}

template<typename X, typename Functor> template<typename Y>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>&
TAO::Utils::Auto_Functor<X,Functor>::
operator=(Auto_Functor<Y,Functor>& rhs)
  throw()
{
  reset(rhs.release());
  return *this;
}
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

template<typename X, typename Functor>
ACE_INLINE X & TAO::Utils::Auto_Functor<X,Functor>::
operator*() const throw()
{
  return *p_;
}

template<typename X, typename Functor>
ACE_INLINE X * TAO::Utils::Auto_Functor<X,Functor>::
operator->() const throw()
{
  return p_;
}

template<typename X, typename Functor>
ACE_INLINE X * TAO::Utils::Auto_Functor<X,Functor>::
get() throw()
{
  return p_;
}

template<typename X, typename Functor>
ACE_INLINE X * TAO::Utils::Auto_Functor<X,Functor>::
release() throw()
{
  X * tmp = p_;
  p_ = 0;
  return tmp;
}

template<typename X, typename Functor>
ACE_INLINE Functor const & TAO::Utils::Auto_Functor<X,Functor>::
functor() const throw()
{
  return f_;
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
Auto_Functor(Auto_Functor_Ref<X,Functor> rhs) throw()
  : p_(rhs.p_)
  , f_(rhs.f_)
{
}

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor> &
TAO::Utils::Auto_Functor<X,Functor>::
operator=(Auto_Functor_Ref<X,Functor> rhs) throw()
{
  if(rhs.p_ != p_)
  {
    reset(rhs.p_);
    f_ = rhs.f_;
  }
  return *this;
}

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)

template<typename X, typename Functor> template<typename Y>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
operator TAO::Utils::Auto_Functor_Ref<Y,Functor>() throw()
{
  return TAO::Utils::Auto_Functor_Ref<Y,Functor>(release(), f_);
}

template<typename X, typename Functor> template<typename Y>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
operator TAO::Utils::Auto_Functor<Y,Functor>() throw()
{
  return TAO::Utils::Auto_Functor<Y,Functor>(release(), f_);
}

#else

template<typename X, typename Functor>
ACE_INLINE TAO::Utils::Auto_Functor<X,Functor>::
operator TAO::Utils::Auto_Functor_Ref<X,Functor>() throw()
{
  return TAO::Utils::Auto_Functor_Ref<X,Functor>(release(), f_);
}

#endif /* ACE_LACKS_MEMBER_TEMPLATES */
