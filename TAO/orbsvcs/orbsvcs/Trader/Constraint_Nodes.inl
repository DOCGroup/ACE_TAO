// $Id$

ACE_INLINE bool
operator!= (const TAO_Literal_Constraint& left,
            const TAO_Literal_Constraint& right)
{
  return !(left == right);
}

ACE_INLINE bool
operator<= (const TAO_Literal_Constraint& left,
            const TAO_Literal_Constraint& right)
{
  return !(left > right);
}

ACE_INLINE bool
operator>= (const TAO_Literal_Constraint& left,
            const TAO_Literal_Constraint& right)
{
  return !(left < right);
}
