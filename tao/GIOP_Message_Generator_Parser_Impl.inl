// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Generator_Parser_Impl::check_revision (
  CORBA::Octet incoming_major,
  CORBA::Octet incoming_minor)
{
  CORBA::UShort const version_as_whole_num =
    incoming_major << 8 | incoming_minor;

  CORBA::UShort const max_allowable_version =
    TAO_DEF_GIOP_MAJOR << 8 | TAO_DEF_GIOP_MINOR;

  // If it's greater than the max, we know it's not allowed.
  if (version_as_whole_num > max_allowable_version)
    return false;

  // If it's less than the max, though, we still have to check for
  // each explicit version and only allow the ones we know work.
  switch (version_as_whole_num)
    {
    case 0x0100:
    case 0x0101:
    case 0x0102:
      return true;
    }

  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
