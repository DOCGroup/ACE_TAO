// $Id$

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (SERVANT *s)
  : ptr_ (s)
{
}

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (const Servant_var<SERVANT> &rhs)
  : ptr_ (rhs.in ())
{
  if (this->ptr_ != 0)
    this->ptr_->_add_ref ();
}

template<class SERVANT> ACE_INLINE Servant_var<SERVANT>&
Servant_var<SERVANT>::operator= (const Servant_var<SERVANT> &rhs)
{
  Servant_var<SERVANT> tmp (rhs);
  {
    // @@ We should use
    //
    //    std::swap (this->ptr_, tmp.ptr_);
    //
    //    but we lack a proper STL on many platforms.
    //
    SERVANT *x = this->ptr_;
    this->ptr_ = tmp.ptr_;
    tmp.ptr_ = x;
  }
  return *this;
}

template<class SERVANT> ACE_INLINE SERVANT*
Servant_var<SERVANT>::operator-> ()
{
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE const SERVANT*
Servant_var<SERVANT>::operator-> () const
{
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::operator SERVANT *()
{
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::operator const SERVANT * () const
{
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE SERVANT *
Servant_var<SERVANT>::in () const
{
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE SERVANT *&
Servant_var<SERVANT>::out ()
{
  this->ptr_->_remove_ref ();
  this->ptr_ = 0;
  return this->ptr_;
}

template<class SERVANT> ACE_INLINE SERVANT *&
Servant_var<SERVANT>::inout ()
{
  return this->ptr_;
}
