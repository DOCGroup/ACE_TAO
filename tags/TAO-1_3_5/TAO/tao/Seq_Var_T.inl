//$Id$

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::TAO_Seq_Var_Base_T (void)
  : ptr_ (0)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::TAO_Seq_Var_Base_T (T * p)
  : ptr_ (p)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::~TAO_Seq_Var_Base_T (void)
{
  delete this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
const T *
TAO_Seq_Var_Base_T<T,T_elem>::operator-> (void) const
{
  return this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T,T_elem>::operator-> (void)
{
  return this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::operator const T & () const
{
  return *this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::operator T & ()
{
  return *this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
TAO_Seq_Var_Base_T<T,T_elem>::operator T & () const
{
  return *this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
const T &
TAO_Seq_Var_Base_T<T,T_elem>::in (void) const
{
  return *this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T &
TAO_Seq_Var_Base_T<T,T_elem>::inout (void)
{
  return *this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T *&
TAO_Seq_Var_Base_T<T,T_elem>::out (void)
{
  delete this->ptr_;
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T,T_elem>::_retn (void)
{
  T * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

template<typename T, typename T_elem>
ACE_INLINE
T *
TAO_Seq_Var_Base_T<T,T_elem>::ptr (void) const
{
  return this->ptr_;
}

// ***************************************************************

template<typename T, typename T_elem>
ACE_INLINE
TAO_FixedSeq_Var_T<T,T_elem>::TAO_FixedSeq_Var_T (void)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_FixedSeq_Var_T<T,T_elem>::TAO_FixedSeq_Var_T (T * p)
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{}

template<typename T, typename T_elem>
TAO_FixedSeq_Var_T<T,T_elem>::TAO_FixedSeq_Var_T (
    const TAO_FixedSeq_Var_T<T,T_elem> & p
  )
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{
}

// Fixed-size base types only.
template<typename T, typename T_elem>
ACE_INLINE
TAO_FixedSeq_Var_T<T,T_elem>::TAO_FixedSeq_Var_T (const T & p)
{
  ACE_NEW (this->ptr_,
           T (p));
}

template<typename T, typename T_elem>
ACE_INLINE
TAO_FixedSeq_Var_T<T,T_elem> &
TAO_FixedSeq_Var_T<T,T_elem>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_elem>
ACE_INLINE
T_elem &
TAO_FixedSeq_Var_T<T,T_elem>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

// ***************************************************************

template<typename T, typename T_elem>
ACE_INLINE
TAO_VarSeq_Var_T<T,T_elem>::TAO_VarSeq_Var_T (void)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_VarSeq_Var_T<T,T_elem>::TAO_VarSeq_Var_T (T * p)
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{}

template<typename T, typename T_elem>
TAO_VarSeq_Var_T<T,T_elem>::TAO_VarSeq_Var_T (
    const TAO_VarSeq_Var_T<T,T_elem> & p
  )
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_VarSeq_Var_T<T,T_elem> &
TAO_VarSeq_Var_T<T,T_elem>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

// Variable-size types only.
template<typename T, typename T_elem>
ACE_INLINE
TAO_VarSeq_Var_T<T,T_elem>::operator T *& ()
{
  return this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T_elem &
TAO_VarSeq_Var_T<T,T_elem>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

// ***************************************************************

template<typename T, typename T_elem>
ACE_INLINE
TAO_MngSeq_Var_T<T,T_elem>::TAO_MngSeq_Var_T (void)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_MngSeq_Var_T<T,T_elem>::TAO_MngSeq_Var_T (T * p)
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{}

template<typename T, typename T_elem>
TAO_MngSeq_Var_T<T,T_elem>::TAO_MngSeq_Var_T (
    const TAO_MngSeq_Var_T<T,T_elem> & p
  )
  : TAO_Seq_Var_Base_T<T,T_elem> (p)
{}

template<typename T, typename T_elem>
ACE_INLINE
TAO_MngSeq_Var_T<T,T_elem> &
TAO_MngSeq_Var_T<T,T_elem>::operator= (T * p)
{
  delete this->ptr_;
  this->ptr_ = p;
  return *this;
}

// Variable-size types only.
template<typename T, typename T_elem>
ACE_INLINE
TAO_MngSeq_Var_T<T,T_elem>::operator T *& ()
{
  return this->ptr_;
}

template<typename T, typename T_elem>
ACE_INLINE
T_elem
TAO_MngSeq_Var_T<T,T_elem>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}
