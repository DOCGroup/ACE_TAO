// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Var_Size_Argument_T<S,Insert_Policy>::In_Var_Size_Argument_T (S const & x)
  : x_ (&x)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
const S &
TAO::In_Var_Size_Argument_T<S,Insert_Policy>::arg (void) const
{
  return *this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::In_Var_Size_Clonable_Argument_T<S,Insert_Policy>::In_Var_Size_Clonable_Argument_T (S const & x)
  : In_Var_Size_Argument_T<S,Insert_Policy> (x),
    is_clone_ (false)
{}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Var_Size_Argument_T<S,Insert_Policy>::Inout_Var_Size_Argument_T (S & x)
  : x_ (&x)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S &
TAO::Inout_Var_Size_Argument_T<S,Insert_Policy>::arg (void)
{
  return *this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Var_Size_Argument_T<S,Insert_Policy>::Out_Var_Size_Argument_T (
    typename S::_out_type x
  )
  : x_ (x.ptr ())
{}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S *&
TAO::Out_Var_Size_Argument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::Ret_Var_Size_Argument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S *&
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S *
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::excp (void)
{
  return this->x_.ptr ();
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S *
TAO::Ret_Var_Size_Argument_T<S,Insert_Policy>::retn (void)
{
  return this->x_._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
