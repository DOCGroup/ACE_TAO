// -*- C++ -*-
//
// $Id$


template <typename StringType,
          typename TypeCodeType,
          class FieldArrayType,
          class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Struct<StringType,
                      TypeCodeType,
                      FieldArrayType,
                      RefCountPolicy>::Struct (
  CORBA::TCKind kind,
  char const * id,
  char const * name,
  FieldArrayType const & fields,
  CORBA::ULong nfields)
  : kind_ (kind)
  , base_attributes_ (id, name)
  , nfields_ (nfields)
  , fields_ (fields)
{
}
