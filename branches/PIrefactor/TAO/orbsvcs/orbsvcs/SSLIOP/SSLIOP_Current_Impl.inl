// -*- C++ -*-
//
// $Id$


ACE_INLINE
TAO::SSLIOP::Current_Impl::Current_Impl (void)
  : ssl_ (0)
{
}

ACE_INLINE void
TAO::SSLIOP::Current_Impl::ssl (SSL *s)
{
  this->ssl_ = s;
}

ACE_INLINE SSL *
TAO::SSLIOP::Current_Impl::ssl (void)
{
  return this->ssl_;
}
