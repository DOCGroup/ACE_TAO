// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Local_Interface<StringType, RefCountPolicy>::Local_Interface (
  char const * id,
  char const * name)
  : Objref<StringType, RefCountPolicy> (id, name)
{
}
