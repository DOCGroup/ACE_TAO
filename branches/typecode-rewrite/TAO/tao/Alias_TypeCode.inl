// -*- C++ -*-
//
// $Id$


template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Alias<StringType, RefCountPolicy>::Alias (
  char const * id,
  char const * name,
  CORBA::TypeCode_ptr * tc)
  : RefCountPolicy (),
    attributes_ (id, name),
    content_type_ (tc)
{
}
