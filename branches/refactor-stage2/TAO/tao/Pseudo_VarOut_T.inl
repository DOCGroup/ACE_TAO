// $Id$

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T (void)
  : ptr_ (T::_nil ())
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T (T * p)
  : ptr_ (p)
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::TAO_Pseudo_Var_T (const TAO_Pseudo_Var_T<T> & p)
  : ptr_ (T::_duplicate (p.ptr ()))
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::~TAO_Pseudo_Var_T (void)
{
  CORBA::release (this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T> &
TAO_Pseudo_Var_T<T>::operator= (T * p)
{
  CORBA::release (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T> &
TAO_Pseudo_Var_T<T>::operator= (const TAO_Pseudo_Var_T<T> & p)
{
  if (this != &p)
    {
      CORBA::release (this->ptr_);
      this->ptr_ = T::_duplicate (p.ptr ());
    }

  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::operator const T *& () const
{
  return ACE_const_cast (const T*, this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Var_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Var_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Var_T<T>::in (void) const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *&
TAO_Pseudo_Var_T<T>::inout (void)
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *&
TAO_Pseudo_Var_T<T>::out (void)
{
  CORBA::release (this->ptr_);
  this->ptr_ = T::_nil ();
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Var_T<T>::_retn (void)
{
  T * val = this->ptr_;
  this->ptr_ = T::_nil ();
  return val;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Var_T<T>::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var>::TAO_Pseudo_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = T::_nil ();
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var>::TAO_Pseudo_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  CORBA::release (this->ptr_);
  this->ptr_ = T::_nil ();
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var>::TAO_Pseudo_Out_T (
    const TAO_Pseudo_Out_T<T,T_var> & p
  )
  : ptr_ (p.ptr_)
{}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var> &
TAO_Pseudo_Out_T<T,T_var>::operator= (const TAO_Pseudo_Out_T<T,T_var> & p)
{
  this->ptr_ = p.ptr_;
  return *this;
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var> &
TAO_Pseudo_Out_T<T,T_var>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Out_T<T,T_var>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_var>
ACE_INLINE
T *&
TAO_Pseudo_Out_T<T,T_var>::ptr (void)
{
  return this->ptr_;
}

template <typename T, typename T_var>
ACE_INLINE
T *
TAO_Pseudo_Out_T<T,T_var>::operator-> (void)
{
  return this->ptr_;
}
