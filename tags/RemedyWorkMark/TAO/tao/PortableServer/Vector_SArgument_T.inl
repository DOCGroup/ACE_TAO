// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<typename S,
         template <typename> class Insert_Policy>
S const &
TAO::In_Vector_SArgument_T<S,Insert_Policy>::arg (void) const
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Inout_Vector_SArgument_T<S,Insert_Policy>::Inout_Vector_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
S &
TAO::Inout_Vector_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Out_Vector_SArgument_T<S,Insert_Policy>::Out_Vector_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
S &
TAO::Out_Vector_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

// ==========================================================================

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
TAO::Ret_Vector_SArgument_T<S,Insert_Policy>::Ret_Vector_SArgument_T (void)
{
}

template<typename S,
         template <typename> class Insert_Policy>
ACE_INLINE
S &
TAO::Ret_Vector_SArgument_T<S,Insert_Policy>::arg (void)
{
  return this->x_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
