// -*- C++ -*-
//
// $Id$

ACE_INLINE void
TAO_SSLIOP_Current_Impl::ssl (SSL *s)
{
  this->ssl_ = s;
}

ACE_INLINE SSL *
TAO_SSLIOP_Current_Impl::ssl (void)
{
  return this->ssl_;
}
