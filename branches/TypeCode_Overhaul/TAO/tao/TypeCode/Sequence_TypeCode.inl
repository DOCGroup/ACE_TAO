// -*- C++ -*-
//
// $Id$

template <class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Array<RefCountPolicy::Array (CORBA::TCKind kind,
                                            CORBA::ULong length,
                                            CORBA::TypeCode_ptr * content_type)
  : kind_ (kind)
  , content_type_ (content_type)
  , length_ (length)
{
  // ACE_ASSERT (kind == CORBA::tk_array || kind == CORBA::tk_sequence);
}
