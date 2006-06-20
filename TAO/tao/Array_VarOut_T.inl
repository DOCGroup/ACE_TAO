// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename array_traits>
ACE_INLINE
TAO_Array_Var_Base_T<array_traits>::TAO_Array_Var_Base_T (void)
  : ptr_ (0)
{}

template<typename array_traits>
ACE_INLINE
TAO_Array_Var_Base_T<array_traits>::TAO_Array_Var_Base_T (slice_type * p)
  : ptr_ (p)
{}

template<typename array_traits>
ACE_INLINE
TAO_Array_Var_Base_T<array_traits>::TAO_Array_Var_Base_T (
    const TAO_Array_Var_Base_T & p
  )
{
  this->ptr_ = TAO::details::array_traits<array_traits>::dup (p.in ());
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Var_Base_T<array_traits>::~TAO_Array_Var_Base_T (void)
{
  TAO::details::array_traits<array_traits>::free (this->ptr_);
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Var_Base_T<array_traits>::operator
typename TAO_Array_Var_Base_T<array_traits>::slice_type * const & () const
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::const_subscript_type
TAO_Array_Var_Base_T<array_traits>::operator[] (CORBA::ULong index) const
{
  return this->ptr_[index];
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::subscript_type
TAO_Array_Var_Base_T<array_traits>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::_in_type
TAO_Array_Var_Base_T<array_traits>::in (void) const
{
  // @todo Replace with C++ cast after vc6 has been dropped,
  // vc6 can't handle this as const cast
  return (const slice_type *) this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::_inout_type
TAO_Array_Var_Base_T<array_traits>::inout (void)
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::_retn_arg_type
TAO_Array_Var_Base_T<array_traits>::_retn_arg (void)
{
  TAO::details::array_traits<array_traits>::free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::_retn_type
TAO_Array_Var_Base_T<array_traits>::_retn (void)
{
  slice_type * tmp = this->ptr_;
  this->ptr_ = 0;
  return tmp;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Var_Base_T<array_traits>::_retn_type
TAO_Array_Var_Base_T<array_traits>::ptr (void) const
{
  return this->ptr_;
}

// *************************************************************

template<typename array_traits>
ACE_INLINE
TAO_FixedArray_Var_T<array_traits>::TAO_FixedArray_Var_T (void)
{}

template<typename array_traits>
ACE_INLINE
TAO_FixedArray_Var_T<array_traits>::TAO_FixedArray_Var_T (_slice_type * p)
  : TAO_Array_Var_Base_T<array_traits> (p)
{}

template<typename array_traits>
ACE_INLINE
TAO_FixedArray_Var_T<array_traits>::TAO_FixedArray_Var_T (
    const TAO_FixedArray_Var_T & p
  )
  : TAO_Array_Var_Base_T<array_traits> (p)
{}


template<typename array_traits>
ACE_INLINE
typename TAO_FixedArray_Var_T<array_traits>::_slice_type *
TAO_FixedArray_Var_T<array_traits>::out (void)
{
  return this->ptr_;
}

// *************************************************************

template<typename array_traits>
ACE_INLINE
TAO_VarArray_Var_T<array_traits>::TAO_VarArray_Var_T (void)
{}

template<typename array_traits>
ACE_INLINE
TAO_VarArray_Var_T<array_traits>::TAO_VarArray_Var_T (_slice_type * p)
  : TAO_Array_Var_Base_T<array_traits> (p)
{}

template<typename array_traits>
ACE_INLINE
TAO_VarArray_Var_T<array_traits>::TAO_VarArray_Var_T (
    const TAO_VarArray_Var_T & p
  )
  : TAO_Array_Var_Base_T<array_traits> (p)
{}

template<typename array_traits>
ACE_INLINE
TAO_VarArray_Var_T<array_traits>::operator
typename TAO_VarArray_Var_T<array_traits>::_slice_type *& ()
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_VarArray_Var_T<array_traits>::_slice_type *&
TAO_VarArray_Var_T<array_traits>::out (void)
{
  TAO::details::array_traits<array_traits>::free (this->ptr_);
  this->ptr_ = 0;
  return this->ptr_;
}

// *************************************************************

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var>::TAO_Array_Out_T (_slice_type *& p)
  : ptr_ (p)
{
  this->ptr_ = 0;
}

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var>::TAO_Array_Out_T (_value_type & p)
  : ptr_ (p.out ())
{
  TAO::details::array_traits<array_traits>::free (this->ptr_);
  this->ptr_ = 0;
}

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var>::TAO_Array_Out_T (
    const TAO_Array_Out_T<array_traits, T_var> & p
  )
  : ptr_ (const_cast<TAO_Array_Out_T<array_traits, T_var> &> (p).ptr_)
{}

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var> &
TAO_Array_Out_T<array_traits, T_var>::operator= (
    const TAO_Array_Out_T<array_traits, T_var> & p
  )
{
  this->ptr_ = const_cast<TAO_Array_Out_T<array_traits, T_var> &> (p).ptr_;
  return *this;
}

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var> &
TAO_Array_Out_T<array_traits, T_var>::operator= (_slice_type *p)
{
  this->ptr_ = p;
  return *this;
}

template<typename array_traits, typename T_var>
ACE_INLINE
TAO_Array_Out_T<array_traits, T_var>::operator
typename TAO_Array_Out_T<array_traits, T_var>::_slice_type *& ()
{
  return this->ptr_;
}

template<typename array_traits, typename T_var>
ACE_INLINE
typename TAO_Array_Out_T<array_traits, T_var>::_slice_type &
TAO_Array_Out_T<array_traits, T_var>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

template<typename array_traits, typename T_var>
ACE_INLINE
typename TAO_Array_Out_T<array_traits, T_var>::_slice_type *&
TAO_Array_Out_T<array_traits, T_var>::ptr (void)
{
  return this->ptr_;
}

// *************************************************************

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::TAO_Array_Forany_T (void)
  : ptr_ (0),
    nocopy_ (false)
{}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::TAO_Array_Forany_T (
    _slice_type * p,
    CORBA::Boolean nocopy
  )
  : ptr_ (p),
    nocopy_ (nocopy)
{}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::TAO_Array_Forany_T (
    const TAO_Array_Forany_T<array_traits> & p
  )
  : ptr_ (p.ptr_),
    nocopy_ (p.nocopy_)
{
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::~TAO_Array_Forany_T (void)
{
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits> &
TAO_Array_Forany_T<array_traits>::operator= (_slice_type * p)
{
  this->ptr_ = p;
  return *this;
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits> &
TAO_Array_Forany_T<array_traits>::operator= (
    const TAO_Array_Forany_T<array_traits> & p
  )
{
  this->ptr_ = p.ptr_;
  this->nocopy_ = p.nocopy_;
  return *this;
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::operator
typename TAO_Array_Forany_T<array_traits>::_slice_type * const & () const
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
TAO_Array_Forany_T<array_traits>::operator
typename TAO_Array_Forany_T<array_traits>::_slice_type *& ()
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
const typename TAO_Array_Forany_T<array_traits>::_slice_type &
TAO_Array_Forany_T<array_traits>::operator[] (CORBA::ULong index) const
{
  const _slice_type & tmp = this->ptr_[index];
  return tmp;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_slice_type &
TAO_Array_Forany_T<array_traits>::operator[] (CORBA::ULong index)
{
  return this->ptr_[index];
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_in_type
TAO_Array_Forany_T<array_traits>::in (void) const
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_inout_type
TAO_Array_Forany_T<array_traits>::inout (void)
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_out_type
TAO_Array_Forany_T<array_traits>::out (void)
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_retn_type
TAO_Array_Forany_T<array_traits>::_retn (void)
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
CORBA::Boolean
TAO_Array_Forany_T<array_traits>::nocopy (void) const
{
  return this->nocopy_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_slice_type *
TAO_Array_Forany_T<array_traits>::ptr (void) const
{
  return this->ptr_;
}

template<typename array_traits>
ACE_INLINE
typename TAO_Array_Forany_T<array_traits>::_slice_type *
TAO_Array_Forany_T<array_traits>::tao_alloc (void)
{
  return TAO::details::array_traits<array_traits>::alloc ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
