// -*- C++ -*-
//
// $Id$

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Struct_Field<STRING_TYPE>::get_name (void) const
{
  return this->name;
}

// -----------------------------------------------------------------
// Some compilers exhibit warnings about better conversion sequence
// from a CORBA::String_var to a char const *.  This member
// specialization works around them by performing explicit
// conversions.
// -----------------------------------------------------------------

template<>
ACE_INLINE char const *
TAO::TypeCode::Struct_Field<CORBA::String_var>::get_name (void) const
{
  return this->name.in ();
}
