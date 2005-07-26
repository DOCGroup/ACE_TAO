// -*- C++ -*-
//
// $Id$

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::
In_Fixed_Array_Argument_T (const S_slice * x)
  : x_ (
#if (defined (_MSC_VER) && _MSC_VER <= 1200) || (defined (__IBMCPP__) && (__IBMCPP__ <= 600))
        // @@ (OO) MSVC++ 6 can't handle the const_cast<> in the
        //         multi-dimensional array case so C-style
        //         "sledgehammer" cast instead (reinterpret_cast<>
        //         doesn't work either).  It's not clear if this is
        //         really the right thing to do but the code won't
        //         compile with MSVC++ 6 without it.
        (S_slice *) x
#else
        const_cast<S_slice *> (x)
#endif  /* _MSC_VER <= 1200 */
        )
{
}

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
const S_slice *
TAO::In_Fixed_Array_Argument_T<S,S_slice,S_forany>::arg (void) const
{
  return this->x_.in ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany>::
Inout_Fixed_Array_Argument_T (S_slice *& x)
  : x_ (x)
{}

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
S_slice *
TAO::Inout_Fixed_Array_Argument_T<S,S_slice,S_forany>::arg (void)
{
  return this->x_.inout ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany>::Out_Fixed_Array_Argument_T (S_slice *&x)
  : x_ (x)
{}

template<typename S,
         typename S_slice,
         typename S_forany>
ACE_INLINE
S_slice *&
TAO::Out_Fixed_Array_Argument_T<S,S_slice,S_forany>::arg (void)
{
  return this->x_.out ();
}

// ===========================================================================

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
ACE_INLINE
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::
Ret_Fixed_Array_Argument_T (void)
{}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
ACE_INLINE
S_slice *&
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::arg (void)
{
  return this->x_._retn_arg ();
}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
ACE_INLINE
S_slice *
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S,
         typename S_slice,
         typename S_var,
         typename S_forany,
         typename S_tag>
ACE_INLINE
S_slice *
TAO::Ret_Fixed_Array_Argument_T<S,S_slice,S_var,S_forany,S_tag>::retn (void)
{
  return this->x_._retn ();
}
