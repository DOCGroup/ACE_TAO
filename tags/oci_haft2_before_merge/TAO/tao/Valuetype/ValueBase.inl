// This may not look like C++, but it's really -*- C++ -*-
//
// $Id$

// Detection of flags in the CDR Stream

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_null_ref (CORBA::ULong tag)
{
  return (tag == 0);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_value_tag (CORBA::ULong tag)
{
  return ((tag & Value_tag_sigbits) == 0x7FFFFF00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: has_codebase_url (CORBA::ULong tag)
{
  return (CORBA::Boolean) (tag & Codebase_url);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_no_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_none);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_single_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_single);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::has_list_type_info (CORBA::ULong tag)
{
  return ((tag & Type_info_sigbits) == Type_info_list);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags:: is_chunked (CORBA::ULong tag)
{
  return (CORBA::Boolean) (tag & 8);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection_tag (CORBA::ULong tag)
{
  return (tag == 0xFFFFFFFFL);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_indirection (CORBA::ULong value)
{
  return (0x80000000L < value && value <= (0xFFFFFFFFL - 4));
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_block_size (CORBA::ULong value)
{
  return ( 0 < value && value < 0x7fffff00L);
}

ACE_INLINE CORBA::Boolean
TAO_OBV_GIOP_Flags::is_end_tag (CORBA::ULong tag)
{
  return (0x80000000L < tag  && tag <= 0xFFFFFFFFL);
}

