// -*- C++ -*-
//
//$Id$

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Var_Base_T<T,T_slice,TAG>::TAO_Array_Var_Base_T (void)
  : ptr_ (0)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Var_Base_T<T,T_slice,TAG>::TAO_Array_Var_Base_T (T_slice * p)
  : ptr_ (p)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Var_Base_T<T,T_slice,TAG>::TAO_Array_Var_Base_T (
    const TAO_Array_Var_Base_T & p
  )
{
  this->ptr_ = TAO::Array_Traits<T,T_slice,TAG>::dup (p.in ());
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Var_Base_T<T,T_slice,TAG>::~TAO_Array_Var_Base_T (void)
{
  TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Var_Base_T<T,T_slice,TAG>::operator T_slice * const & () const
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice const &
TAO_Array_Var_Base_T<T,T_slice,TAG>::operator[] (CORBA::ULong index) const
{
  return this->ptr_[index];
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice &
TAO_Array_Var_Base_T<T,T_slice,TAG>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
const T_slice *
TAO_Array_Var_Base_T<T,T_slice,TAG>::in (void) const
{
  return (const T_slice *) this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Var_Base_T<T,T_slice,TAG>::inout (void)
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *&
TAO_Array_Var_Base_T<T,T_slice,TAG>::out (void)
{
  TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Var_Base_T<T,T_slice,TAG>::_retn (void)
{
  T_slice * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Var_Base_T<T,T_slice,TAG>::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_FixedArray_Var_T<T,T_slice,TAG>::TAO_FixedArray_Var_T (void)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_FixedArray_Var_T<T,T_slice,TAG>::TAO_FixedArray_Var_T (T_slice * p)
  : TAO_Array_Var_Base_T<T,T_slice,TAG> (p)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_FixedArray_Var_T<T,T_slice,TAG>::TAO_FixedArray_Var_T (
    const TAO_FixedArray_Var_T & p
  )
  : TAO_Array_Var_Base_T<T,T_slice,TAG> (p)
{}

// *************************************************************

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_VarArray_Var_T<T,T_slice,TAG>::TAO_VarArray_Var_T (void)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_VarArray_Var_T<T,T_slice,TAG>::TAO_VarArray_Var_T (T_slice * p)
  : TAO_Array_Var_Base_T<T,T_slice,TAG> (p)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_VarArray_Var_T<T,T_slice,TAG>::TAO_VarArray_Var_T (
    const TAO_VarArray_Var_T & p
  )
  : TAO_Array_Var_Base_T<T,T_slice,TAG> (p)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_VarArray_Var_T<T,T_slice,TAG>::operator T_slice *& ()
{
  return this->ptr_;
}

// *************************************************************

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG>::TAO_Array_Out_T (T_slice *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG>::TAO_Array_Out_T (T_var & p)
  : ptr_ (p.out ())
{
  TAO::Array_Traits<T,T_slice,TAG>::free (this->ptr_);
  this->ptr_ = 0;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG>::TAO_Array_Out_T (
    const TAO_Array_Out_T<T,T_var,T_slice,TAG> & p
  )
  : ptr_ (const_cast<THIS_OUT_TYPE &> (p).ptr_)
{}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG> &
TAO_Array_Out_T<T,T_var,T_slice,TAG>::operator= (
    const TAO_Array_Out_T<T,T_var,T_slice,TAG> & p
  )
{
  this->ptr_ = const_cast<THIS_OUT_TYPE &> (p).ptr_;
  return *this;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG> &
TAO_Array_Out_T<T,T_var,T_slice,TAG>::operator= (T_slice *p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Out_T<T,T_var,T_slice,TAG>::operator T_slice *& ()
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
T_slice *&
TAO_Array_Out_T<T,T_var,T_slice,TAG>::ptr (void)
{
  return this->ptr_;
}

template<typename T, typename T_var, typename T_slice, typename TAG>
ACE_INLINE
T_slice &
TAO_Array_Out_T<T,T_var,T_slice,TAG>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

// *************************************************************

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::TAO_Array_Forany_T (void)
  : ptr_ (0),
    nocopy_ (0)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::TAO_Array_Forany_T (
    T_slice * p,
    CORBA::Boolean nocopy
  )
  : ptr_ (p),
    nocopy_ (nocopy)
{}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::TAO_Array_Forany_T (
    const TAO_Array_Forany_T<T,T_slice,TAG> & p
  )
  : ptr_ (p.ptr_),
    nocopy_ (p.nocopy_)
{
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::~TAO_Array_Forany_T (void)
{
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG> &
TAO_Array_Forany_T<T,T_slice,TAG>::operator= (T_slice * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG> &
TAO_Array_Forany_T<T,T_slice,TAG>::operator= (
    const TAO_Array_Forany_T<T,T_slice,TAG> & p
  )
{
  this->ptr_ = p.ptr_;
  this->nocopy_ = p.nocopy_;
  return *this;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::operator T_slice * const & () const
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
TAO_Array_Forany_T<T,T_slice,TAG>::operator T_slice *& ()
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
const T_slice &
TAO_Array_Forany_T<T,T_slice,TAG>::operator[] (CORBA::ULong index) const
{
#if defined (ACE_HAS_BROKEN_IMPLICIT_CONST_CAST)
# if defined (_MSC_VER) && _MSC_VER <= 1200
  // @@ (OO) MSVC++ 6 can't handle the const_cast<> in the
  //         multi-dimensional array case so reinterpret_cast<> cast
  //         instead.  It's not clear if this is really the right
  //         thing to do but the code won't compile with MSVC++ 6
  //         without it.  We use a reinterpret_cast<> instead of a
  //         C-style "sledgehammer" cast to make it obvious that this
  //         code may have unresolved issues, and also to make it
  //         easier for others to find this code.
  return reinterpret_cast<const T_slice &> (this->ptr_[index]);
# else
  return const_cast<const T_slice &> (this->ptr_[index]);
# endif  /* _MSC_VER <= 1200 */
#else
  const T_slice & tmp = this->ptr_[index];
  return tmp;
#endif /* ACE_HAS_BROKEN_IMPLICIT_CONST_CAST */
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice &
TAO_Array_Forany_T<T,T_slice,TAG>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
const T_slice *
TAO_Array_Forany_T<T,T_slice,TAG>::in (void) const
{
  // @@@ (JP) This looks scary I know but it helps MSVC understand
  // things better when the array is multi-dimensional.
#if defined (_MSC_VER) && _MSC_VER <= 1200
  // @@ (OO) MSVC++ 6 can't handle the const_cast<> in the
  //         multi-dimensional array case so reinterpret_cast<> cast
  //         instead.  It's not clear if this is really the right
  //         thing to do but the code won't compile with MSVC++ 6
  //         without it.  We use a reinterpret_cast<> instead of a
  //         C-style "sledgehammer" cast to make it obvious that this
  //         code may have unresolved issues, and also to make it
  //         easier for others to find this code.
  return reinterpret_cast<const T_slice *> (this->ptr_);
#else
  return const_cast<const T_slice *> (this->ptr_);
#endif  /* _MSC_VER <= 1200 */
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Forany_T<T,T_slice,TAG>::inout (void)
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *&
TAO_Array_Forany_T<T,T_slice,TAG>::out (void)
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Forany_T<T,T_slice,TAG>::_retn (void)
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Forany_T<T,T_slice,TAG>::ptr (void) const
{
  return this->ptr_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
CORBA::Boolean
TAO_Array_Forany_T<T,T_slice,TAG>::nocopy (void) const
{
  return this->nocopy_;
}

template<typename T, typename T_slice, typename TAG>
ACE_INLINE
T_slice *
TAO_Array_Forany_T<T,T_slice,TAG>::tao_alloc (void)
{
  return TAO::Array_Traits<T,T_slice,TAG>::alloc ();
}
