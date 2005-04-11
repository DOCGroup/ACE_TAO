// -*- C++ -*-
//
// $Id$

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
TAO::TypeCode::Non_Default_Case<DiscriminatorType,
                                StringType,
                                TypeCodeType>::Non_Default_Case (
  DiscriminatorType member_label,
  char const * member_name,
  TypeCodeType member_type)
  : Case<StringType, TypeCodeType> (member_name, member_type)
  , label_ (member_label)
{
}
