ACE_INLINE
IIOP_ServerRequest::IIOP_ServerRequest(CDR *msg,
                                       CORBA_ORB_ptr the_orb,
                                       CORBA_BOA_ptr the_boa)
  : _incoming (msg),
    _params (0), 
    _retval (0),
    _exception (0),
    _ex_type (NO_EXCEPTION),
    refcount_ (1),
    _orb (the_orb),
    _boa (the_boa)
{}
