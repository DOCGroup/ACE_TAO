// -*- C++ -*-
// $Id$

// @@ TODO TAO allocates its ServerRequest objects from the stack,
// using reference counting wouldn't help much.  Using a _clone()
// method would make the following methods really easy to implement,
// but that's hard to implement for the extremely optimized
// TAO_GIOP_ServerRequest.
// Another solution would be to modify the class hierarchy, make
// ServerRequest the "slow" version (with copies for each field) and
// TAO_GIOP_ServerRequest the "fast" version (with no copies at all). The
// first would be use for DII and the later for SII.

ACE_INLINE TAO_InputCDR &
TAO_GIOP_ServerRequest::incoming (void)
{
  return *this->incoming_;
}

ACE_INLINE TAO_OutputCDR &
TAO_GIOP_ServerRequest::outgoing (void)
{
  return *this->outgoing_;
}

// Invocation attributes.

ACE_INLINE const char *
TAO_GIOP_ServerRequest::operation (void) const
{
  return this->operation_.c_str ();
}

ACE_INLINE void
TAO_GIOP_ServerRequest::operation (ACE_CString &operation)
{
  this->operation_ = operation;
}
// get the length of the operation name.
ACE_INLINE unsigned int
TAO_GIOP_ServerRequest::operation_length (void) const
{
  return this->operation_.length ();
}

#if 0
//@@ (TAO) to do after Context is implemented
ACE_INLINE CORBA::Context_ptr
TAO_GIOP_ServerRequest::ctx (void)
{
  return 0;
}
#endif

ACE_INLINE CORBA::Boolean
TAO_GIOP_ServerRequest::response_expected (void) const
{
  return this->response_expected_;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::
response_expected (CORBA::Boolean response)
{
  this->response_expected_ = response;
}

ACE_INLINE CORBA::Boolean
TAO_GIOP_ServerRequest::sync_with_server (void) const
{
  return this->sync_with_server_;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::sync_with_server (CORBA::Boolean sync_flag)
{
  this->sync_with_server_ = sync_flag;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::_tao_lazy_evaluation (int lazy_evaluation)
{
  this->lazy_evaluation_ = lazy_evaluation;
}

ACE_INLINE CORBA::Principal_ptr
TAO_GIOP_ServerRequest::principal (void) const
{
  return this->requesting_principal_.ptr ();
}

ACE_INLINE TAO_ObjectKey &
TAO_GIOP_ServerRequest::object_key (void)
{
  return this->object_key_;
}

ACE_INLINE CORBA::Object_ptr
TAO_GIOP_ServerRequest::objref (CORBA_Environment &ACE_TRY_ENV)
{
  return this->orb ()->key_to_object (this->object_key_,
                                      0,
                                      0,
                                      1,
                                      ACE_TRY_ENV);
}

ACE_INLINE IOP::ServiceContextList &
TAO_GIOP_ServerRequest::service_info (void)
{
  return this->service_info_;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::
service_info (IOP::ServiceContextList &service_info) 
{
  this->service_info_ = service_info;
}

ACE_INLINE CORBA::ULong
TAO_GIOP_ServerRequest::request_id (void)
{
  return this->request_id_;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::request_id (CORBA::ULong req)
{
  this->request_id_ = req;
}

ACE_INLINE void
TAO_GIOP_ServerRequest::requesting_principal (CORBA_Principal_ptr
                                              principal)
{
  this->requesting_principal_ = principal;
}

//ACE_INLINE void
//TAO_GIOP_ServerRequest::header_length (size_t len)
//{
//  this->header_len_ = len;
//}
// 
//ACE_INLINE void
//TAO_GIOP_ServerRequest::message_size_offset (size_t len)
//{
//  this->message_size_offset_ = len;
//}
