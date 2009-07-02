// -*- C++ -*-
//
// $Id$


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
ACE_INLINE
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::Case_T (DiscriminatorType member_label,
                                             char const * member_name,
                                             TypeCodeType member_type)
  : Case<StringType, TypeCodeType> (member_name, member_type)
  , label_ (member_label)
{
}

template <typename DiscriminatorType,
          typename StringType,
          typename TypeCodeType>
ACE_INLINE
TAO::TypeCode::Case_T<DiscriminatorType,
                      StringType,
                      TypeCodeType>::Case_T (DiscriminatorType member_label)
  : Case<StringType, TypeCodeType> ()
  , label_ (member_label)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
