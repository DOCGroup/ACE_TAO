// -*- C++ -*-
//
// $Id$

template <typename STRING_TYPE>
ACE_INLINE
TAO::TypeCode::Base_Attributes<STRING_TYPE>::Base_Attributes (
  char const * id,
  char const * name)
  : id_ (id),
    name_ (name)
{
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<STRING_TYPE>::id (void) const
{
  return this->id_;
}

template <typename STRING_TYPE>
ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<STRING_TYPE>::name (void) const
{
  return this->name_;
}

// -----------------------------------------------------------------
// Some compilers exhibit warnings about better conversion sequence
// from a CORBA::String_var to a char const *.  These member
// specializations work around them by performing explicit
// conversions.
// -----------------------------------------------------------------

ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<CORBA::String_var>::id (void) const
{
  return this->id_.in ();
}

ACE_INLINE char const *
TAO::TypeCode::Base_Attributes<CORBA::String_var>::name (void) const
{
  return this->name_.in ();
}
