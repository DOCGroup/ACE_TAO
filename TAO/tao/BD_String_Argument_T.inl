// $Id$

template<typename S, typename to_S, typename from_S, size_t BOUND>
ACE_INLINE
TAO::In_BD_String_Argument_T<S,to_S,from_S,BOUND>::In_BD_String_Argument_T (
    const S * x
  )
  : x_ (x)
{}

// ==========================================================================

template<typename S, typename to_S, typename from_S, size_t BOUND>
ACE_INLINE
TAO::Inout_BD_String_Argument_T<S,to_S,from_S,BOUND>::
Inout_BD_String_Argument_T (S *& x)
  : x_ (x)
{}

// ==========================================================================

template<typename S, 
         typename S_out, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::Out_BD_String_Argument_T<S,S_out,to_S,from_S,BOUND>::
Out_BD_String_Argument_T (S_out x)
  : x_ (x.ptr ())
{}

// ==========================================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::
Ret_BD_String_Argument_T (void)
{
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
S *
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
S *
TAO::Ret_BD_String_Argument_T<S,S_var,to_S,from_S,BOUND>::retn (void)
{
  return this->x_._retn ();
}

// ==========================================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
In_BD_String_SArgument_T (void)
{}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
const S *
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Inout_BD_String_SArgument_T (void)
{}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
S *&
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::Out_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Out_BD_String_SArgument_T (void)
{}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
S *&
TAO::Out_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::
Ret_BD_String_SArgument_T (void)
{}

template<typename S, 
         typename S_var, 
         typename to_S, 
         typename from_S, 
         size_t BOUND>
ACE_INLINE
S *&
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND>::arg (void)
{
  return this->x_.out ();
}

