// -*- C++ -*-
//
// $Id$

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Default_Case<STRING_TYPE>::Default_Case (
  char const * member_name,
  CORBA::TypeCode_ptr * member_type)
  : Case<STRING_TYPE> (member_name, member_type)
{
}
