// $Id$

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::TAO_Pseudo_Value_Var_T (void)
  : ptr_ (0)
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::TAO_Pseudo_Value_Var_T (T * p)
  : ptr_ (p)
{}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::TAO_Pseudo_Value_Var_T (const TAO_Pseudo_Value_Var_T<T> & p)
  : ptr_ (p.ptr_)
{
  CORBA::add_ref (this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::~TAO_Pseudo_Value_Var_T (void)
{
  CORBA::remove_ref (this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T> &
TAO_Pseudo_Value_Var_T<T>::operator= (T * p)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = p;
  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T> &
TAO_Pseudo_Value_Var_T<T>::operator= (const TAO_Pseudo_Value_Var_T<T> & p)
{
  if (this != &p)
    {
      CORBA::remove_ref (this->ptr_);
      this->ptr_ = p.ptr ();
      CORBA::add_ref (this->ptr_);
    }

  return *this;
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::operator const T *& () const
{
  return ACE_const_cast (const T*, this->ptr_);
}

template <typename T>
ACE_INLINE
TAO_Pseudo_Value_Var_T<T>::operator T *& ()
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Value_Var_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Value_Var_T<T>::in (void) const
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *&
TAO_Pseudo_Value_Var_T<T>::inout (void)
{
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *&
TAO_Pseudo_Value_Var_T<T>::out (void)
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Value_Var_T<T>::_retn (void)
{
  T * val = this->ptr_;
  this->ptr_ = 0;
  return val;
}

template <typename T>
ACE_INLINE
T *
TAO_Pseudo_Value_Var_T<T>::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var>::TAO_Pseudo_Value_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = T::_nil ();
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var>::TAO_Pseudo_Value_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  CORBA::remove_ref (this->ptr_);
  this->ptr_ = T::_nil ();
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var>::TAO_Pseudo_Value_Out_T (
    const TAO_Pseudo_Value_Out_T<T,T_var> & p
  )
  : ptr_ (p.ptr_)
{}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var> &
TAO_Pseudo_Value_Out_T<T,T_var>::operator= (
    const TAO_Pseudo_Value_Out_T<T,T_var> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var> &
TAO_Pseudo_Value_Out_T<T,T_var>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template <typename T, typename T_var>
ACE_INLINE
TAO_Pseudo_Value_Out_T<T,T_var>::operator T *& ()
{
  return this->ptr_;
}

template <typename T, typename T_var>
ACE_INLINE
T *&
TAO_Pseudo_Value_Out_T<T,T_var>::ptr (void)
{
  return this->ptr_;
}

template <typename T, typename T_var>
ACE_INLINE
T *
TAO_Pseudo_Value_Out_T<T,T_var>::operator-> (void)
{
  return this->ptr_;
}
