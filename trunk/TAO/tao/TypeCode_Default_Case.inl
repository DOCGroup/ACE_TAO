// -*- C++ -*-
//
// $Id$

template <typename StringType, typename TypeCodeType>
TAO::TypeCode::Default_Case<StringType, TypeCodeType>::Default_Case (
  char const * member_name,
  TypeCodeType member_type)
  : Case<StringType, TypeCodeType> (member_name, member_type)
{
}
