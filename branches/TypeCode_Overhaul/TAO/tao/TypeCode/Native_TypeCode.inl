// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Native<StringType, RefCountPolicy>::Native (char const * id,
                                                           char const * name)
  : Objref<StringType, RefCountPolicy> (id, name)
{
}
