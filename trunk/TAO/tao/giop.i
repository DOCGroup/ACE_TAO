// This may look like C, but it's really -*- C++ -*-

ACE_INLINE
TAO_GIOP_RequestHeader::TAO_GIOP_RequestHeader (void)
  : request_id(0),
    response_expected(CORBA::B_FALSE)
{}

// Initialize the request header from <msg>, setting <env> for errors.
ACE_INLINE CORBA::Boolean
TAO_GIOP_RequestHeader::init (CDR &msg, CORBA::Environment &env)
{
  CORBA::Boolean hdr_status;

  // Tear out the service context ... we currently ignore it,
  // but it should probably be passed to each ORB service as
  // appropriate (e.g. transactions, security).
  //
  // NOTE: As security support kicks in, this is a good place to
  // verify a digital signature, if that is required in this
  // security environment.  It may be required even when using
  // IPSEC security infrastructure.

  hdr_status = msg.decode (&TC_ServiceContextList,
                           &this->service_info,
                           0, 
                           env);

  // Get the rest of the request header ...

  hdr_status = hdr_status && msg.get_ulong (this->request_id);
  hdr_status = hdr_status && msg.get_boolean (this->response_expected);
  hdr_status = hdr_status && msg.decode (&TC_opaque, 
                                         &this->object_key,
                                         0, 
                                         env);
  hdr_status = hdr_status && msg.decode (CORBA::_tc_string,
                                         &this->operation,
                                         0,
                                         env);
  hdr_status = hdr_status && msg.decode (CORBA::_tc_Principal,
                                         &this->requesting_principal,
                                         0,
                                         env);

  return hdr_status;
}

ACE_INLINE CORBA::Boolean
TAO_GIOP::start_message (TAO_GIOP_MsgType type, CDR &msg)
{
  msg.next = msg.buffer;		// for reused streams
  msg.remaining = msg.length;

  if (msg.bytes_remaining () < TAO_GIOP_HEADER_LEN)
    return CORBA::B_FALSE;

  msg.next [0] = 'G';
  msg.next [1] = 'I';
  msg.next [2] = 'O';
  msg.next [3] = 'P';

  msg.next [4] = MY_MAJOR;
  msg.next [5] = MY_MINOR;
  msg.next [6] = MY_BYTE_SEX;
  msg.next [7] = (u_char) type;

  msg.skip_bytes (TAO_GIOP_HEADER_LEN);
  return CORBA::B_TRUE;
}

ACE_INLINE void 
TAO_GIOP_Invocation::get_value (CORBA::TypeCode_ptr tc,
                                void *value,
                                CORBA::Environment &env)
{
  (void) stream.decode (tc, value, 0, env);
}

ACE_INLINE void 
TAO_GIOP_Invocation::put_param (CORBA::TypeCode_ptr tc, 
                                void *value, 
                                CORBA::Environment &env)
{
  (void) stream.encode (tc, value, 0, env);
}
