//$Id$

ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Generator_Parser_Impl::
    check_revision (CORBA::Octet incoming_major,
                    CORBA::Octet incoming_minor)
{
#if 0
  if (incoming_major > TAO_DEF_GIOP_MAJOR ||
      incoming_minor > TAO_DEF_GIOP_MINOR)
    return 0;

  return 1;
#endif
  CORBA::Boolean ret = 0; // Assume that the revision is bogus
  if (incoming_major == 1)
    switch (incoming_minor)
      {
      case 0:
      case 1:
      case 2:
        ret = 1;
      }

  return ret;
}
