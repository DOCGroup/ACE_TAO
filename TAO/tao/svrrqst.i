ACE_INLINE
IIOP_ServerRequest::IIOP_ServerRequest(CDR *msg,
                                       CORBA::ORB_ptr the_orb,
                                       CORBA::POA_ptr the_boa)
  : incoming_ (msg),
    params_ (0), 
    retval_ (0),
    exception_ (0),
    ex_type_ (CORBA::NO_EXCEPTION),
    refcount_ (1),
    orb_ (the_orb),
    boa_ (the_boa)
{}
