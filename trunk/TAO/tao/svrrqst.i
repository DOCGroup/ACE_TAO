ACE_INLINE
IIOP_ServerRequest::IIOP_ServerRequest(CDR *msg,
                                       CORBA::ORB_ptr the_orb,
                                       CORBA::BOA_ptr the_boa)
  : _incoming (msg),
    _params (0), 
    _retval (0),
    _exception (0),
    _ex_type (CORBA::NO_EXCEPTION),
    refcount_ (1),
    _orb (the_orb),
    _boa (the_boa)
{}
