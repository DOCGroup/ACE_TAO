// -*- C++ -*-
//
// $Id$

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_EVP_PKEY::_duplicate (EVP_PKEY *key)
{
  // OpenSSL provides no function to increase the reference count on
  // the EVP_PKEY structure, so we do it manually.
  if (key != 0)
    CRYPTO_add (&(key->references), 1, CRYPTO_LOCK_EVP_PKEY);

  return key;
}

ACE_INLINE void
TAO_SSLIOP_EVP_PKEY::release (EVP_PKEY *key)
{
  // OpenSSL's EVP_PKEY_free() function already handles reference
  // counting properly.
  ::EVP_PKEY_free (key);
}


// -------------------------------------------------------------------

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::TAO_SSLIOP_EVP_PKEY_var (void)
  : evp_ (0)
{
}

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::TAO_SSLIOP_EVP_PKEY_var (EVP_PKEY *x)
  : evp_ (x)
{
}

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::TAO_SSLIOP_EVP_PKEY_var (const ::TAO_SSLIOP_EVP_PKEY_var &p)
  : TAO_Base_var (),
    evp_ (TAO_SSLIOP_EVP_PKEY::_duplicate (p.ptr ()))
{
}

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::~TAO_SSLIOP_EVP_PKEY_var (void)
{
  TAO_SSLIOP_EVP_PKEY::release (this->evp_);
}

ACE_INLINE TAO_SSLIOP_EVP_PKEY_var &
TAO_SSLIOP_EVP_PKEY_var::operator= (EVP_PKEY *p)
{
  TAO_SSLIOP_EVP_PKEY::release (this->evp_);
  this->evp_ = p;
  return *this;
}

ACE_INLINE ::TAO_SSLIOP_EVP_PKEY_var &
TAO_SSLIOP_EVP_PKEY_var::operator= (const ::TAO_SSLIOP_EVP_PKEY_var &p)
{
  if (this != &p)
    {
      TAO_SSLIOP_EVP_PKEY::release (this->evp_);
      this->evp_ = TAO_SSLIOP_EVP_PKEY::_duplicate (p.ptr ());
    }

  return *this;
}

ACE_INLINE const EVP_PKEY *
TAO_SSLIOP_EVP_PKEY_var::operator-> (void) const
{
  return this->evp_;
}

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_EVP_PKEY_var::operator-> (void)
{
  return this->evp_;
}

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::operator const EVP_PKEY &() const
{
  return *this->evp_;
}

ACE_INLINE
TAO_SSLIOP_EVP_PKEY_var::operator EVP_PKEY &()
{
  return *this->evp_;
}

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_EVP_PKEY_var::in (void) const
{
  return this->evp_;
}

ACE_INLINE EVP_PKEY *&
TAO_SSLIOP_EVP_PKEY_var::inout (void)
{
  return this->evp_;
}

ACE_INLINE EVP_PKEY *&
TAO_SSLIOP_EVP_PKEY_var::out (void)
{
  EVP_PKEY_free (this->evp_);
  this->evp_ = 0;
  return this->evp_;
}

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_EVP_PKEY_var::_retn (void)
{
  // Yield ownership of the EVP_PKEY structure.
  EVP_PKEY *x = this->evp_;
  this->evp_ = 0;
  return x;
}

ACE_INLINE EVP_PKEY *
TAO_SSLIOP_EVP_PKEY_var::ptr (void) const
{
  return this->evp_;
}
