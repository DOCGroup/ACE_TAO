// -*- C++ -*-
//
//$Id$

ACE_INLINE
TAO_Operation_Details::TAO_Operation_Details (const char *name,
                                              CORBA::ULong len,
                                              CORBA::Boolean argument_flag)
  : opname_ (name),
    opname_len_ (len),
    request_id_ (0),
    argument_flag_ (argument_flag),
    response_flags_ (0),
    addressing_mode_ (TAO_Target_Specification::Key_Addr)
{
}

ACE_INLINE const char*
TAO_Operation_Details::opname (void) const
{
  return this->opname_;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::opname_len (void) const
{
  return this->opname_len_;
}

ACE_INLINE CORBA::Boolean
TAO_Operation_Details::argument_flag (void) const
{
  return this->argument_flag_;
}

ACE_INLINE TAO_Service_Context &
TAO_Operation_Details::request_service_context (void)
{
  return this->request_service_info_;
}

ACE_INLINE const TAO_Service_Context &
TAO_Operation_Details::request_service_context (void) const
{
  return this->request_service_info_;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Operation_Details::request_service_info (void)
{
  // @@ This should go ...
  return this->request_service_context ().service_info ();
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Operation_Details::request_service_info (void) const
{
  // @@ This should go ...
  return this->request_service_context ().service_info ();
}

ACE_INLINE TAO_Service_Context &
TAO_Operation_Details::reply_service_context (void)
{
  return this->reply_service_info_;
}

ACE_INLINE const TAO_Service_Context &
TAO_Operation_Details::reply_service_context (void) const
{
  return this->reply_service_info_;
}

ACE_INLINE IOP::ServiceContextList &
TAO_Operation_Details::reply_service_info (void)
{
  // @@ This should go ...
  return this->reply_service_context ().service_info ();
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Operation_Details::reply_service_info (void) const
{
  // @@ This should go ...
  return this->reply_service_context ().service_info ();
}

ACE_INLINE void
TAO_Operation_Details::request_id (CORBA::ULong id)
{
  this->request_id_ = id;
}

ACE_INLINE void
TAO_Operation_Details::modify_request_id (int originator)
{
  // originator ==  1 --> originating side
  // originator ==  0 --> other side
  // originator == -1 --> no bi-directional connection was negotiated

  // The originating side must have an even request ID, and the other
  // side must have an odd request ID.  Make sure that is the case.
  if ((originator == 1 && ACE_ODD (this->request_id_))
      || (originator == 0 && ACE_EVEN (this->request_id_)))
    ++(this->request_id_);
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::request_id (void)
{
  return this->request_id_;
}

ACE_INLINE CORBA::ULong
TAO_Operation_Details::request_id (void) const
{
  return this->request_id_;
}

ACE_INLINE void
TAO_Operation_Details::response_flags (CORBA::Octet flags)
{
  this->response_flags_ = flags;
}

ACE_INLINE CORBA::Octet
TAO_Operation_Details::response_flags (void)
{
  return this->response_flags_;
}

ACE_INLINE CORBA::Octet
TAO_Operation_Details::response_flags (void) const
{
  return this->response_flags_;
}

ACE_INLINE TAO_Target_Specification::TAO_Target_Address
TAO_Operation_Details::addressing_mode (void)
{
  return this->addressing_mode_;
}

ACE_INLINE TAO_Target_Specification::TAO_Target_Address
TAO_Operation_Details::addressing_mode (void) const
{
  return this->addressing_mode_;
}

ACE_INLINE void
TAO_Operation_Details::addressing_mode (CORBA::Short mode)
{
  if (mode == 0)
    this->addressing_mode_ = TAO_Target_Specification::Key_Addr;
  else if (mode == 1)
    this->addressing_mode_ = TAO_Target_Specification::Profile_Addr;
  else if (mode == 2)
    this->addressing_mode_ = TAO_Target_Specification::Reference_Addr;
}
