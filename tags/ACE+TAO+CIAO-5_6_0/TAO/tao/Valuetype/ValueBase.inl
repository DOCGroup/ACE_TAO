// -*- C++ -*-
//
// $Id$

// Detection of flags in the CDR Stream

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_null_ref (CORBA::Long tag)
{
  return (tag == Null_tag);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_value_tag (CORBA::Long tag)
{
  return ((tag & Value_tag_sigbits) == 0x7FFFFF00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: has_codebase_url (CORBA::Long tag)
{
  return (CORBA::Boolean) (tag & Codebase_url);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_no_type_info (CORBA::Long tag)
{
  return ((tag & Type_info_sigbits) == Type_info_none);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_single_type_info (CORBA::Long tag)
{
  return ((tag & Type_info_sigbits) == Type_info_single);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_list_type_info (CORBA::Long tag)
{
  return ((tag & Type_info_sigbits) == Type_info_list);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: is_chunked (CORBA::Long tag)
{
  return (CORBA::Boolean) (tag & 8);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection_tag (CORBA::Long tag)
{
  return (static_cast<unsigned>(tag) == 0xFFFFFFFFL);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection (CORBA::Long value)
{
  return (0x80000000L < static_cast<unsigned>(value) &&
          static_cast<unsigned>(value) <= (0xFFFFFFFFL - 4));
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_block_size (CORBA::Long value)
{
  return (0 < static_cast<unsigned>(value) &&
          static_cast<unsigned>(value) < 0x7FFFFF00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_end_tag (CORBA::Long tag)
{
  return (0x80000000L < (unsigned)tag);
}

TAO_END_VERSIONED_NAMESPACE_DECL
