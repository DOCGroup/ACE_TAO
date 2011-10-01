// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Fixed_Array_Argument_T<S_forany,Insert_Policy>::
In_Fixed_Array_Argument_T (const typename S_forany::_slice_type * x)
  : x_ (
#if defined __IBMCPP__ && __IBMCPP__ <= 800
        // @@ (OO) IBMCPP can't handle the const_cast<> in the
        //         multi-dimensional array case so C-style
        //         "sledgehammer" cast instead (reinterpret_cast<>
        //         doesn't work either).  It's not clear if this is
        //         really the right thing to do but the code won't
        //         compile without it.
        (typename S_forany::_slice_type *) x
#else
        const_cast<typename S_forany::_slice_type *> (x)
#endif  /* IBMCPP */
        )
{
}

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
const typename S_forany::_slice_type *
TAO::In_Fixed_Array_Argument_T<S_forany,Insert_Policy>::arg (void) const
{
  return this->x_.in ();
}

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Fixed_Array_Clonable_Argument_T<S_forany,Insert_Policy>::
In_Fixed_Array_Clonable_Argument_T (const typename S_forany::_slice_type * x)
  : In_Fixed_Array_Argument_T<S_forany,Insert_Policy> (x),
    is_clone_ (false)
{
}


// ===========================================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Fixed_Array_Argument_T<S_forany,Insert_Policy>::
Inout_Fixed_Array_Argument_T (typename S_forany::_slice_type *& x)
  : x_ (x)
{}

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Inout_Fixed_Array_Argument_T<S_forany,Insert_Policy>::arg (void)
{
  return this->x_.inout ();
}

// ===========================================================================

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Fixed_Array_Argument_T<
    S_forany,
    Insert_Policy
  >::Out_Fixed_Array_Argument_T (typename S_forany::_slice_type *&x)
  : x_ (x)
{}

template<typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *&
TAO::Out_Fixed_Array_Argument_T<S_forany,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ===========================================================================

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::
Ret_Fixed_Array_Argument_T (void)
{}

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *&
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::arg (void)
{
  return this->x_._retn_arg ();
}

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S_var,
         typename S_forany,
         template <typename> class Insert_Policy>
ACE_INLINE
typename S_forany::_slice_type *
TAO::Ret_Fixed_Array_Argument_T<S_var,S_forany,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
