// -*- C++ -*-
//$Id$
template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem>::TAO_Seq_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem>::TAO_Seq_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem>::TAO_Seq_Out_T (
    const TAO_Seq_Out_T<T,T_var,T_elem> & p
  )
  : ptr_ (p.ptr_)
{}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem> &
TAO_Seq_Out_T<T,T_var,T_elem>::operator= (
    const TAO_Seq_Out_T<T,T_var,T_elem> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem> &
TAO_Seq_Out_T<T,T_var,T_elem>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_Seq_Out_T<T,T_var,T_elem>::operator T *& ()
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T *&
TAO_Seq_Out_T<T,T_var,T_elem>::ptr (void)
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T *
TAO_Seq_Out_T<T,T_var,T_elem>::operator-> (void)
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T_elem &
TAO_Seq_Out_T<T,T_var,T_elem>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

// ****************************************************************
template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem>::TAO_MngSeq_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem>::TAO_MngSeq_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem>::TAO_MngSeq_Out_T (
    const TAO_MngSeq_Out_T<T,T_var,T_elem> & p
  )
  : ptr_ (p.ptr_)
{}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem> &
TAO_MngSeq_Out_T<T,T_var,T_elem>::operator= (
    const TAO_MngSeq_Out_T<T,T_var,T_elem> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem> &
TAO_MngSeq_Out_T<T,T_var,T_elem>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
TAO_MngSeq_Out_T<T,T_var,T_elem>::operator T *& ()
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T *&
TAO_MngSeq_Out_T<T,T_var,T_elem>::ptr (void)
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T *
TAO_MngSeq_Out_T<T,T_var,T_elem>::operator-> (void)
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_elem>
ACE_INLINE
T_elem
TAO_MngSeq_Out_T<T,T_var,T_elem>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}
