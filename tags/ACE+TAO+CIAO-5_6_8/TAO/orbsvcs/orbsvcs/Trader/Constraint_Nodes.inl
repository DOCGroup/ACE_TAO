// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

TAO_END_VERSIONED_NAMESPACE_DECL
