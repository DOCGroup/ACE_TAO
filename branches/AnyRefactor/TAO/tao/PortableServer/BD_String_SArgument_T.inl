// $Id$

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
In_BD_String_SArgument_T (void)
{}

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
const S *
TAO::In_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
arg (void) const
{
  return this->x_.in ();
}

// ==========================================================================

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
Inout_BD_String_SArgument_T (void)
{}

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
S *&
TAO::Inout_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
arg (void)
{
  return this->x_.inout ();
}

// ==========================================================================

template<typename S,
         typename S_var,
         typename S_out,
         typename to_S,
         typename from_S,
         size_t BOUND>
ACE_INLINE
TAO::Out_BD_String_SArgument_T<S,S_var,S_out,to_S,from_S,BOUND,Insert_Policy>::
Out_BD_String_SArgument_T (void)
{}

template<typename S,
         typename S_var,
         typename S_out,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
S *&
TAO::Out_BD_String_SArgument_T<S,S_var,S_out,to_S,from_S,BOUND,Insert_Policy>::
arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
Ret_BD_String_SArgument_T (void)
{}

template<typename S,
         typename S_var,
         typename to_S,
         typename from_S,
         size_t BOUND,
         typename Insert_Policy>
ACE_INLINE
S *&
TAO::Ret_BD_String_SArgument_T<S,S_var,to_S,from_S,BOUND,Insert_Policy>::
arg (void)
{
  return this->x_.out ();
}

