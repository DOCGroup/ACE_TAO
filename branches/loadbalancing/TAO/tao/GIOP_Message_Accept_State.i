// -*- C++ -*-
//$Id$


///////////////////////////////////////////////////////////////
// TAO_GIOP_Message_Accept_Impl methods
//////////////////////////////////////////////////////////////
  
ACE_INLINE CORBA::Boolean
TAO_GIOP_Message_Accept_Impl::check_revision (CORBA::Octet incoming_major,
                                              CORBA::Octet incoming_minor)
{
  // Need a better way to do this. @@Bala
  if (incoming_major > 1 ||
      incoming_minor > 1)
    return 0;

  return 1;
}

