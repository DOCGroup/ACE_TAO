// -*- C++ -*-
//
// $Id$


template<typename StringType,
         typename TypeCodeType,
         CORBA::TCKind Kind,
         class RefCountPolicy>
ACE_INLINE
TAO::TypeCode::Alias<StringType,
                     TypeCodeType,
                     Kind,
                     RefCountPolicy>::Alias (
  char const * id,
  char const * name,
  TypeCodeType const & tc)
  : RefCountPolicy (),
    attributes_ (id, name),
    content_type_ (tc)
{
}
