// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::Recursive_Type (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  MemberArrayType const & fields,
  CORBA::ULong nfields)
  : TypeCodeBase (kind, id, name, fields, nfields)
  , lock_ ()
  , recursion_start_offset_ (0)
  , data_initialized_(true)
{
  // ACE_ASSERT (kind != CORBA::tk_except);
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::Recursive_Type (
  char const * id,
  char const * name,
  TypeCodeType const & discriminant_type,
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
  , recursion_start_offset_ (0)
  , data_initialized_(true)
{
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::Recursive_Type (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  CORBA::ValueModifier modifier,
  TypeCodeType const & concrete_base,
  MemberArrayType const & fields,
  CORBA::ULong nfields)
  : TypeCodeBase (kind,
                  id,
                  name,
                  modifier,
                  concrete_base,
                  fields,
                  nfields)
  , lock_ ()
  , recursion_start_offset_ (0)
  , data_initialized_(true)
{
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::Recursive_Type (
  CORBA::TCKind kind,
  char const * id)
  : TypeCodeBase (kind, id)
  , lock_ ()
  , recursion_start_offset_ (0)
  , data_initialized_(false)
{
//   ACE_ASSERT (kind == CORBA::tk_struct
//               || kind == CORBA::tk_union
//               || kind == CORBA::tk_value
//               || kind == CORBA::tk_event);
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::struct_parameters (
  char const * name,
  MemberArrayType const & fields,
  CORBA::ULong nfields)
{
  // Do not replace pre-existing fields!
  if ( !this->data_initialized_ )
    {
      this->base_attributes_.name (name);
      this->fields_  = fields;
      this->nfields_ = nfields;
      this->data_initialized_ = true;

      return true;
    }

  return false;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::union_parameters (
  char const * name,
  TypeCodeType const & discriminant_type,
  MemberArrayType const & cases,
  CORBA::ULong ncases,
  CORBA::Long default_index)
{
  if ( !this->data_initialized_ )
    {
      this->base_attributes_.name (name);
      this->discriminant_type_ = discriminant_type;
      this->cases_             = cases;
      this->ncases_            = ncases;
      this->default_index_     = default_index;
      this->data_initialized_ = true;

      return true;
    }

  return false;
}

template <class TypeCodeBase, typename TypeCodeType, typename MemberArrayType>
ACE_INLINE bool
TAO::TypeCode::Recursive_Type<TypeCodeBase,
                              TypeCodeType,
                              MemberArrayType>::valuetype_parameters (
  char const * name,
  CORBA::ValueModifier modifier,
  TypeCodeType const & concrete_base,
  MemberArrayType const & fields,
  CORBA::ULong nfields)
{
  if ( !this->data_initialized_ )
    {
      this->base_attributes_.name (name);
      this->type_modifier_ = modifier;
      this->concrete_base_ = concrete_base;
      this->fields_        = fields;
      this->nfields_       = nfields;
      this->data_initialized_ = true;

      return true;
    }

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
