// -*- C++ -*-
//
// $Id$

ACE_INLINE EVP_KEY *
TAO_SSLIOP_EVP_KEY::_duplicate (EVP_KEY *key)
{
  // OpenSSL provides no function to increase the reference count on
  // the EVP_KEY structure, so we do it manually.
  if (key != 0)
    CRYPTO_add (&(key->references), 1, CRYPTO_LOCK_EVP_KEY);

  return key;
}

ACE_INLINE void
TAO_SSLIOP_EVP_KEY::release (EVP_KEY *key)
{
  // OpenSSL's EVP_KEY_free() function already handles reference
  // counting properly.
  ::EVP_KEY_free (key);
}


// -------------------------------------------------------------------

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::TAO_SSLIOP_EVP_KEY_var (void)
  : evp_key_ (0)
{
}

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::TAO_SSLIOP_EVP_KEY_var (EVP_KEY *x)
  : evp_key_ (x)
{
}

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::TAO_SSLIOP_EVP_KEY_var (const ::TAO_SSLIOP_EVP_KEY_var &p)
  : TAO_Base_var (),
    evp_key_ (TAO_SSLIOP_EVP_KEY::_duplicate (p.ptr ()))
{
}

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::~TAO_SSLIOP_EVP_KEY_var (void)
{
  TAO_SSLIOP_EVP_KEY::release (this->evp_key_);
}

ACE_INLINE TAO_SSLIOP_EVP_KEY_var &
TAO_SSLIOP_EVP_KEY_var::operator= (EVP_KEY *p)
{
  TAO_SSLIOP_EVP_KEY::release (this->evp_key_);
  this->evp_key_ = p;
  return *this;
}

ACE_INLINE ::TAO_SSLIOP_EVP_KEY_var &
TAO_SSLIOP_EVP_KEY_var::operator= (const ::TAO_SSLIOP_EVP_KEY_var &p)
{
  if (this != &p)
    {
      TAO_SSLIOP_EVP_KEY::release (this->evp_key_);
      this->evp_key_ = TAO_SSLIOP_EVP_KEY::_duplicate (p.ptr ());
    }

  return *this;  
}

ACE_INLINE const EVP_KEY *
TAO_SSLIOP_EVP_KEY_var::operator-> (void) const
{
  return this->evp_key_;
}

ACE_INLINE EVP_KEY *
TAO_SSLIOP_EVP_KEY_var::operator-> (void)
{
  return this->evp_key_;
}

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::operator const EVP_KEY &() const
{
  return *this->evp_key_;
}

ACE_INLINE
TAO_SSLIOP_EVP_KEY_var::operator EVP_KEY &()
{
  return *this->evp_key_;
}

ACE_INLINE EVP_KEY *
TAO_SSLIOP_EVP_KEY_var::in (void) const
{
  return this->evp_key_;
}

ACE_INLINE EVP_KEY *&
TAO_SSLIOP_EVP_KEY_var::inout (void)
{
  return this->evp_key_;
}

ACE_INLINE EVP_KEY *&
TAO_SSLIOP_EVP_KEY_var::out (void)
{
  EVP_KEY_free (this->evp_key_);
  this->evp_key_ = 0;
  return this->evp_key_;
}

ACE_INLINE EVP_KEY *
TAO_SSLIOP_EVP_KEY_var::_retn (void)
{
  // Yield ownership of the EVP_KEY structure.
  EVP_KEY *x = this->evp_key_;
  this->evp_key_ = 0;
  return x;
}

ACE_INLINE EVP_KEY *
TAO_SSLIOP_EVP_KEY_var::ptr (void) const
{
  return this->evp_key_;
}
