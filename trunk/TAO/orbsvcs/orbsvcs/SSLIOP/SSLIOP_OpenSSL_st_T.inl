// -*- C++ -*-
//
// $Id$


#include <openssl/crypto.h>


template <typename T> ACE_INLINE T *
TAO::SSLIOP::_duplicate (T * st)
{
  // Shallow copy.

  // OpenSSL provides no structure-specific functions to increase the
  // reference count on the structure it defines, so we do it
  // manually.
  if (st != 0)
    CRYPTO_add (&(st->references),
                1,
                TAO::SSLIOP::OpenSSL_traits<T>::LOCK_ID);

  return st;
}

template <typename T> ACE_INLINE T *
TAO::SSLIOP::copy (T const & st)
{
  // Deep copy.
  return TAO::SSLIOP::OpenSSL_traits<T>::copy (st);
}

template <typename T> ACE_INLINE void
TAO::SSLIOP::release (T * st)
{
  TAO::SSLIOP::OpenSSL_traits<T>::release (st);
}

// -------------------------------------------------------------------

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::OpenSSL_st_var (void)
  : TAO_Base_var (),
    st_ (0)
{
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::OpenSSL_st_var (T * st)
  : TAO_Base_var (),
    st_ (st)
{
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::OpenSSL_st_var (
  TAO::SSLIOP::OpenSSL_st_var<T> const & st)
  : TAO_Base_var (),
    st_ (TAO::SSLIOP::_duplicate (st.ptr ()))
{
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::OpenSSL_st_var (T const & st)
  : TAO_Base_var (),
    st_ (TAO::SSLIOP::copy (st))
{
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::~OpenSSL_st_var (void)
{
  TAO::SSLIOP::release (this->st_);
}

template <typename T> ACE_INLINE TAO::SSLIOP::OpenSSL_st_var<T> &
TAO::SSLIOP::OpenSSL_st_var<T>::operator= (T * st)
{
  TAO::SSLIOP::release (this->st_);
  this->st_ = st;
  return *this;
}

template <typename T> ACE_INLINE TAO::SSLIOP::OpenSSL_st_var<T> &
TAO::SSLIOP::OpenSSL_st_var<T>::operator= (
  TAO::SSLIOP::OpenSSL_st_var<T> const & st)
{
  if (this != &st)
    {
      TAO::SSLIOP::release (this->st_);
      this->st_ = TAO::SSLIOP::_duplicate (st.ptr ());
    }

  return *this;
}

template <typename T> ACE_INLINE TAO::SSLIOP::OpenSSL_st_var<T> &
TAO::SSLIOP::OpenSSL_st_var<T>::operator= (T const & st)
{
  if (this->st_ != &st)
    {
      TAO::SSLIOP::release (this->st_);
      this->st_ = TAO::SSLIOP::copy (st);
    }

  return *this;
}

template <typename T> ACE_INLINE T const *
TAO::SSLIOP::OpenSSL_st_var<T>::operator-> (void) const
{
  return this->st_;
}

template <typename T> ACE_INLINE T *
TAO::SSLIOP::OpenSSL_st_var<T>::operator-> (void)
{
  return this->st_;
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::operator T const &() const
{
  return *this->st_;
}

template <typename T> ACE_INLINE
TAO::SSLIOP::OpenSSL_st_var<T>::operator T &()
{
  return *this->st_;
}

template <typename T> ACE_INLINE T *
TAO::SSLIOP::OpenSSL_st_var<T>::in (void) const
{
  return this->st_;
}

template <typename T> ACE_INLINE T *&
TAO::SSLIOP::OpenSSL_st_var<T>::inout (void)
{
  return this->st_;
}

template <typename T> ACE_INLINE T *&
TAO::SSLIOP::OpenSSL_st_var<T>::out (void)
{
  TAO::SSLIOP::release (this->st_);
  this->st_ = 0;
  return this->st_;
}

template <typename T> ACE_INLINE T *
TAO::SSLIOP::OpenSSL_st_var<T>::_retn (void)
{
  // Yield ownership of the OpenSSL structure.
  T * st = this->st_;
  this->st_ = 0;
  return st;
}

template <typename T> ACE_INLINE T *
TAO::SSLIOP::OpenSSL_st_var<T>::ptr (void) const
{
  return this->st_;
}
