/**
 * @file Servant_var.inl
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<typename SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (SERVANT *s)
  : ptr_ (s)
{
}

template<typename SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (const Servant_var<SERVANT> &rhs)
  : ptr_ (rhs.ptr_)
{
  if (this->ptr_ != 0)
    this->ptr_->_add_ref ();
}

template<typename SERVANT> ACE_INLINE Servant_var<SERVANT>&
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

template<typename SERVANT> ACE_INLINE SERVANT*
Servant_var<SERVANT>::operator-> ()
{
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE const SERVANT*
Servant_var<SERVANT>::operator-> () const
{
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE
Servant_var<SERVANT>::operator SERVANT *()
{
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE
Servant_var<SERVANT>::operator const SERVANT * () const
{
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE SERVANT *
Servant_var<SERVANT>::in ()
{
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE SERVANT *&
Servant_var<SERVANT>::out ()
{
  this->ptr_->_remove_ref ();
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename SERVANT> ACE_INLINE SERVANT *&
Servant_var<SERVANT>::inout ()
{
  return this->ptr_;
}
