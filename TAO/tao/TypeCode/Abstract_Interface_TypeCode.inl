// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Abstract_Interface<StringType,
                                  RefCountPolicy>::Abstract_Interface (
  char const * id,
  char const * name)
  : Objref<StringType, RefCountPolicy> (id, name)
{
}
