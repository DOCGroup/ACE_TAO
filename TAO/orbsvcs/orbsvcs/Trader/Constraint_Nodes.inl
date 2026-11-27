TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
operator!= (TAO_Literal_Constraint const& left,
            TAO_Literal_Constraint const& right)
{
  return !(left == right);
}

ACE_INLINE bool
operator<= (TAO_Literal_Constraint const& left,
            TAO_Literal_Constraint const& right)
{
  return !(left > right);
}

ACE_INLINE bool
operator>= (TAO_Literal_Constraint const& left,
            TAO_Literal_Constraint const& right)
{
  return !(left < right);
}

TAO_END_VERSIONED_NAMESPACE_DECL
