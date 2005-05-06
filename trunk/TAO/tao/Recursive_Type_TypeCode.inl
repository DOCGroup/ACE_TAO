// -*- C++ -*-
//
// $Id$

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::RecursiveType (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  MemberArrayType const & fields,
  CORBA::ULong nfields)
  : TypeCodeBase (kind, id, name, fields, nfields)
  , lock_ ()
  , starting_offset_ (0)
  , in_recursion_ (false)
{
  // ACE_ASSERT (kind != CORBA::tk_except);
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::RecursiveType (
  char const * id,
  char const * name,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
  TypeCodeType discriminant_type,
#else
  TypeCodeType const & discriminant_type,
#endif
  MemberArrayType const & cases,
  CORBA::ULong ncases,
  CORBA::Long default_index)
  : TypeCodeBase (id,
                  name,
                  discriminant_type,
                  cases,
                  ncases,
                  default_index)
  , lock_ ()
  , starting_offset_ (0)
  , in_recursion_ (false)
{
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::RecursiveType (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  CORBA::ValueModifier modifier,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
  TypeCodeType concrete_base,
#else
  TypeCodeType const & concrete_base,
#endif
  MemberArrayType const & fields,
  CORBA::ULong nfields)
  TypeCodeBase (kind,
                id,
                name,
                modifier,
                concrete_base,
                fields,
                nfields)
  , lock_ ()
  , starting_offset_ (0)
  , in_recursion_ (false)
{
}
