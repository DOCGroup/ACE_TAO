//$Id$

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Generator_Parser_Impl::
    check_revision (CORBA::Octet incoming_major,
                    CORBA::Octet incoming_minor)
{
  if (incoming_major > TAO_DEF_GIOP_MAJOR ||
      incoming_minor > TAO_DEF_GIOP_MINOR)
    return 0;

  return 1;
}
