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
  // @@ Bala: could you be more precise? What needs to be better?
  // @@ Bala: does this means that we reject GIOP 1.2 messages? I
  // think you are right to do so, because we don't export GIOP 1.2
  // IORs....
  if (incoming_major > TAO_DEF_GIOP_MAJOR ||
      incoming_minor > TAO_DEF_GIOP_MINOR)
    return 0;

  return 1;
}
