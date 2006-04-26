// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::
In_Fixed_Array_Argument_T (const S_slice * x)
  : x_ (
#if (defined (__IBMCPP__) && (__IBMCPP__ <= 600))
        // @@ (OO) IMBCPP 6 can't handle the const_cast<> in the
        //         multi-dimensional array case so C-style
        //         "sledgehammer" cast instead (reinterpret_cast<>
        //         doesn't work either).  It's not clear if this is
        //         really the right thing to do but the code won't
        //         compile without it.
        (S_slice *) x
#else
        const_cast<S_slice *> (x)
#endif  /* IBMCPP <= 600 */
        )
{
}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
const S_slice *
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::arg (void) const
{
  return this->x_.in ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::
Inout_Fixed_Array_Argument_T (S_slice *& x)
  : x_ (x)
{}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice *
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::arg (void)
{
  return this->x_.inout ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::Out_Fixed_Array_Argument_T (S_slice *&x)
  : x_ (x)
{}

template<typename S,
         typename S_slice,
         typename S_forany,
         typename Insert_Policy>
ACE_INLINE
S_slice *&
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
ACE_INLINE
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::
Ret_Fixed_Array_Argument_T (void)
{}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
ACE_INLINE
S_slice *&
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::arg (void)
{
  return this->x_._retn_arg ();
}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
ACE_INLINE
S_slice *
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag,
         typename Insert_Policy>
ACE_INLINE
S_slice *
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
