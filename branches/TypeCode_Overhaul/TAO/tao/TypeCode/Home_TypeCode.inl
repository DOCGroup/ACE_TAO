// -*- C++ -*-
//
// $Id$


template <typename StringType, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Home<StringType, RefCountPolicy>::Home (char const * id,
                                                       char const * name)
  : Objref<StringType, RefCountPolicy> (id, name)
{
}
