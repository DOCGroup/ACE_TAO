//$Id$
// -*- C++ -*-
ACE_INLINE
TAO_Operation_Details::TAO_Operation_Details (const char *name,
                                              CORBA::ULong len)
  :opname_ (name),
   opname_len_ (len),
   request_id_ (0),
   response_flags_ (0)
{
  //no-op
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

ACE_INLINE IOP::ServiceContextList &
TAO_Operation_Details::service_info (void)
{
  return this->service_info_;
}

ACE_INLINE const IOP::ServiceContextList &
TAO_Operation_Details::service_info (void) const
{
  return this->service_info_;
}

ACE_INLINE void
TAO_Operation_Details::request_id (CORBA::ULong id)
{
  this->request_id_ = id;
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
