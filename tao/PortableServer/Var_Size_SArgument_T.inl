// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
S const &
TAO::In_Var_Size_SArgument_T<S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Var_Size_SArgument_T<S,Insert_Policy>::Inout_Var_Size_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
S &
TAO::Inout_Var_Size_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Var_Size_SArgument_T<S,Insert_Policy>::Out_Var_Size_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
S *&
TAO::Out_Var_Size_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Var_Size_SArgument_T<S,Insert_Policy>::Ret_Var_Size_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S *&
TAO::Ret_Var_Size_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_.out ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
