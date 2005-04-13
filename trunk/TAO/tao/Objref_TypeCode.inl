// -*- C++ -*-
//
// $Id$


template <typename StringType, CORBA::TCKind Kind, class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Objref<StringType,
                      Kind,
                      RefCountPolicy>::Objref (char const * id,
                                               char const * name)
  : Objref_Base<StringType, RefCountPolicy> (Kind, id, name)
{
}
