/**
 * @file Servant_var.inl
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

template<class SERVANT> ACE_INLINE SERVANT *
Servant_var<SERVANT>::duplicate (SERVANT *servant)
{
  if (servant != 0)
    servant->_add_ref ();
  return servant;
}

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (SERVANT *s)
  : ptr_ (s)
{
}

template<class SERVANT> ACE_INLINE
Servant_var<SERVANT>::Servant_var (const Servant_var<SERVANT> &rhs)
  : ptr_ (Servant_var<SERVANT>::duplicate (rhs.ptr_))
{
}

template<class SERVANT> ACE_INLINE Servant_var<SERVANT>&
Servant_var<SERVANT>::operator= (const Servant_var<SERVANT> &rhs)
{
  Servant_var<SERVANT> tmp (rhs);
  ACE_Swap<SERVANT*>::swap (this->ptr_, tmp.ptr_);
  return *this;
}

template<class SERVANT> ACE_INLINE Servant_var<SERVANT>&
Servant_var<SERVANT>::operator= (SERVANT *rhs)
{
  Servant_var<SERVANT> tmp (rhs);
  ACE_Swap<SERVANT*>::swap (this->ptr_, tmp.ptr_);
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
  Servant_var<SERVANT> tmp;
  ACE_Swap<SERVANT*>::swap (tmp.ptr_, this->ptr_);

  return this->ptr_;
}

template<class SERVANT> ACE_INLINE SERVANT *&
Servant_var<SERVANT>::inout ()
{
  return this->ptr_;
}
