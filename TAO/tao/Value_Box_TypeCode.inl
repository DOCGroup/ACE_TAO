// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Value_Box<StringType, RefCountPolicy>::Value_Box (
  char const * id,
  char const * name,
  CORBA::TypeCode_ptr const * tc)
  : RefCountPolicy (),
    attributes_ (id, name),
    content_type_ (tc)
{
}
