//$Id$

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::TAO_Var_Base_T (void)
  : ptr_ (0)
{}

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::TAO_Var_Base_T (T * p)
  : ptr_ (p)
{}

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::~TAO_Var_Base_T (void)
{
  delete this->ptr_;
}

template<typename T>
ACE_INLINE 
const T *
TAO_Var_Base_T<T>::operator-> (void) const
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE 
T *
TAO_Var_Base_T<T>::operator-> (void)
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::operator const T & () const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::operator T & () 
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE
TAO_Var_Base_T<T>::operator T & () const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE 
const T &
TAO_Var_Base_T<T>::in (void) const
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE 
T &
TAO_Var_Base_T<T>::inout (void)
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE 
T *
TAO_Var_Base_T<T>::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************

template<typename T>
ACE_INLINE
TAO_Fixed_Var_T<T>::TAO_Fixed_Var_T (void)
{}

template<typename T>
ACE_INLINE
TAO_Fixed_Var_T<T>::TAO_Fixed_Var_T (T * p)
  : TAO_Var_Base_T<T> (p)
{}

template<typename T>
ACE_INLINE
TAO_Fixed_Var_T<T>::TAO_Fixed_Var_T (const TAO_Fixed_Var_T<T> & p)
  : TAO_Var_Base_T<T> (p)
{}

// Fixed-size types only.
template<typename T>
ACE_INLINE
TAO_Fixed_Var_T<T>::TAO_Fixed_Var_T (const T & p)
{
  ACE_NEW (this->ptr_, 
           T (p));
}

template<typename T>
ACE_INLINE
TAO_Fixed_Var_T<T> &
TAO_Fixed_Var_T<T>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

// Mapping for fixed size.
template<typename T>
ACE_INLINE 
T &
TAO_Fixed_Var_T<T>::out (void)
{
  return *this->ptr_;
}

template<typename T>
ACE_INLINE 
T
TAO_Fixed_Var_T<T>::_retn (void)
{
  return *this->ptr_;
}

// *************************************************************

template<typename T>
ACE_INLINE
TAO_Var_Var_T<T>::TAO_Var_Var_T (void)
{}

template<typename T>
ACE_INLINE
TAO_Var_Var_T<T>::TAO_Var_Var_T (T * p)
  : TAO_Var_Base_T<T> (p)
{}

template<typename T>
ACE_INLINE
TAO_Var_Var_T<T>::TAO_Var_Var_T (const TAO_Var_Var_T<T> & p)
  : TAO_Var_Base_T<T> (p)
{}

template<typename T>
ACE_INLINE
TAO_Var_Var_T<T> &
TAO_Var_Var_T<T>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

// Variable-size types only.
template<typename T>
ACE_INLINE
TAO_Var_Var_T<T>::operator T *& ()
{
  return this->ptr_;
}

// Mapping for variable size.
template<typename T>
ACE_INLINE
T *&
TAO_Var_Var_T<T>::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename T>
ACE_INLINE
T *
TAO_Var_Var_T<T>::_retn (void)
{
  T * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

// *************************************************************

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var>::TAO_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var>::TAO_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var>::TAO_Out_T (const TAO_Out_T<T,T_var> & p)
  : ptr_ (ACE_const_cast (THIS_OUT_TYPE &, p).ptr_)
{}

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var> &
TAO_Out_T<T,T_var>::operator= (const TAO_Out_T<T,T_var> & p)
{
  this->ptr_ = ACE_const_cast (THIS_OUT_TYPE &, p).ptr_;
  return *this;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var> &
TAO_Out_T<T,T_var>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_var>
ACE_INLINE
TAO_Out_T<T,T_var>::operator T *& ()
{
  return this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE
T *&
TAO_Out_T<T,T_var>::ptr (void)
{
  return this->ptr_;
}

template<typename T, typename T_var>
ACE_INLINE
T *
TAO_Out_T<T,T_var>::operator-> (void)
{
  return this->ptr_;
}

