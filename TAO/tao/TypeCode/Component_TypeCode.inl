// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Component<StringType, RefCountPolicy>::Component (
  char const * id,
  char const * name)
  : Objref<StringType, RefCountPolicy> (id, name)
{
}
