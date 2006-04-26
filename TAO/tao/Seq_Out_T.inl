// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T>::TAO_Seq_Out_T (T *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T>::TAO_Seq_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  delete this->ptr_;
  this->ptr_ = 0;
}

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T>::TAO_Seq_Out_T (
    const TAO_Seq_Out_T<T> & p
  )
  : ptr_ (p.ptr_)
{}

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T> &
TAO_Seq_Out_T<T>::operator= (
    const TAO_Seq_Out_T<T> & p
  )
{
  this->ptr_ = p.ptr_;
  return *this;
}

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T> &
TAO_Seq_Out_T<T>::operator= (T * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T>
ACE_INLINE
TAO_Seq_Out_T<T>::operator T *& ()
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
T *&
TAO_Seq_Out_T<T>::ptr (void)
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
T *
TAO_Seq_Out_T<T>::operator-> (void)
{
  return this->ptr_;
}

template<typename T>
ACE_INLINE
typename TAO_Seq_Out_T<T>::T_elem
TAO_Seq_Out_T<T>::operator[] (CORBA::ULong index)
{
  return this->ptr_->operator[] (index);
}

TAO_END_VERSIONED_NAMESPACE_DECL
