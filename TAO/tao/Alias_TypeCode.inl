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
#ifdef __BORLANDC__
  // Borland C++ currently can't handle a reference to
  // const pointer to const CORBA::TypeCode_ptr
  TypeCodeType tc)
#else
  TypeCodeType const & tc)
#endif
  : CORBA::TypeCode (Kind)
  , RefCountPolicy ()
  , attributes_ (id, name)
  , content_type_ (tc)
{
}
