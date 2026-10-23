// -*- C++ -*-

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO::SSLIOP::Current_Impl::ssl (SSL *s)
{
  this->ssl_ = s;
}

ACE_INLINE SSL *
TAO::SSLIOP::Current_Impl::ssl ()
{
  return this->ssl_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
