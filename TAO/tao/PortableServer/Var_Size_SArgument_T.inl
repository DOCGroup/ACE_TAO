// -*- C++ -*-
//
// $Id$

template<typename S,
         typename Insert_Policy>
S const &
TAO::In_Var_Size_SArgument_T<S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         typename Insert_Policy>
ACE_INLINE
TAO::Inout_Var_Size_SArgument_T<S,Insert_Policy>::Inout_Var_Size_SArgument_T (void)
{
}

template<typename S,
         typename Insert_Policy>
S &
TAO::Inout_Var_Size_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         typename S_var,
         typename Insert_Policy>
ACE_INLINE
TAO::Out_Var_Size_SArgument_T<S,S_var,Insert_Policy>::Out_Var_Size_SArgument_T (void)
{
}

template<typename S,
         typename S_var,
         typename Insert_Policy>
S *&
TAO::Out_Var_Size_SArgument_T<S,S_var,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S,
         typename S_var,
         typename Insert_Policy>
ACE_INLINE
TAO::Ret_Var_Size_SArgument_T<S,S_var,Insert_Policy>::Ret_Var_Size_SArgument_T (void)
{
}

template<typename S,
         typename S_var,
         typename Insert_Policy>
ACE_INLINE
S *&
TAO::Ret_Var_Size_SArgument_T<S,S_var,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

