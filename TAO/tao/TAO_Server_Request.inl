// -*- C++ -*-
#include "tao/GIOP_Utils.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// This constructor is used by the CSD clone.
ACE_INLINE
TAO_ServerRequest::TAO_ServerRequest ()
  : mesg_base_ (0),
    operation_ (0),
    operation_len_ (0),
    release_operation_ (false),
    is_forwarded_ (false),
    incoming_ (0),
    outgoing_ (0),
    response_expected_ (false),
    deferred_reply_ (false),
    sync_with_server_ (false),
    is_queued_ (false),
    is_dsi_ (false),
    reply_status_ (GIOP::NO_EXCEPTION),
    orb_core_ (0),
    request_id_ (0),
    profile_ (0),
    requesting_principal_ (0),
    dsi_nvlist_align_ (0),
    operation_details_ (0),
    argument_flag_ (true)
#if TAO_HAS_INTERCEPTORS == 1
  , interceptor_count_ (0)
  , rs_pi_current_ ()
  , caught_exception_ (0)
  , pi_reply_status_ (-1)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
  ,  transport_(0)
{
}

ACE_INLINE TAO_ORB_Core *
TAO_ServerRequest::orb_core () const
{
  return this->orb_core_;
}

ACE_INLINE TAO_InputCDR *
TAO_ServerRequest::incoming () const
{
  return this->incoming_;
}

ACE_INLINE TAO_OutputCDR *
TAO_ServerRequest::outgoing () const
{
  return this->outgoing_;
}

ACE_INLINE const char *
TAO_ServerRequest::operation () const
{
  return (this->operation_ == 0 ? "" : this->operation_);
}

ACE_INLINE void
TAO_ServerRequest::operation (const char *operation,
                              size_t length,
                              int release)
{
  if (this->release_operation_)
    CORBA::string_free (const_cast <char*> (this->operation_));

  this->operation_len_ = (length == 0 ? std::strlen (operation) : length);
  this->release_operation_ = release;
  this->operation_ = operation;
}

ACE_INLINE size_t
TAO_ServerRequest::operation_length () const
{
  return this->operation_len_;
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequest::response_expected () const
{
  return this->response_expected_;
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequest::deferred_reply () const
{
  return this->deferred_reply_;
}

ACE_INLINE void
TAO_ServerRequest::response_expected (CORBA::Boolean response)
{
  this->response_expected_ = response;
}

ACE_INLINE void
TAO_ServerRequest::sync_before_dispatch ()
{
  if (this->sync_with_server_ &&
      this->transport_ != 0 &&
      this->is_queued_)
    {
      this->send_no_exception_reply ();
    }
}

ACE_INLINE void
TAO_ServerRequest::sync_after_dispatch ()
{
  if (this->sync_with_server_ &&
      this->transport_ != 0 &&
      !this->is_queued_)
    {
      this->send_no_exception_reply ();
    }
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequest::sync_with_server () const
{
  return this->sync_with_server_;
}

ACE_INLINE void
TAO_ServerRequest::sync_with_server (CORBA::Boolean sync_flag)
{
  this->sync_with_server_ = sync_flag;
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequest::is_queued () const
{
  return this->is_queued_;
}

ACE_INLINE void
TAO_ServerRequest::is_queued (CORBA::Boolean queued_flag)
{
  this->is_queued_ = queued_flag;
}

ACE_INLINE TAO::ObjectKey &
TAO_ServerRequest::object_key ()
{
  return this->profile_.object_key ();
}

ACE_INLINE TAO_Service_Context &
TAO_ServerRequest::request_service_context ()
{
  return this->request_service_context_;
}

ACE_INLINE IOP::ServiceContextList &
TAO_ServerRequest::reply_service_info ()
{
  return this->reply_service_context ().service_info ();
}

ACE_INLINE IOP::ServiceContextList &
TAO_ServerRequest::request_service_info ()
{
  return this->request_service_context ().service_info ();
}

ACE_INLINE TAO_Transport *
TAO_ServerRequest::transport ()
{
  return this->transport_.get ();
}

ACE_INLINE CORBA::ULong
TAO_ServerRequest::request_id ()
{
  return this->request_id_;
}

ACE_INLINE void
TAO_ServerRequest::request_id (CORBA::ULong req)
{
  this->request_id_ = req;
}

ACE_INLINE void
TAO_ServerRequest::requesting_principal (const CORBA::OctetSeq &principal)
{
  this->requesting_principal_ = principal;
}

ACE_INLINE TAO_Tagged_Profile &
TAO_ServerRequest::profile ()
{
  return this->profile_;
}

ACE_INLINE void
TAO_ServerRequest::forward_location (CORBA::Object_ptr forward_reference)
{
  this->forward_location_ =
    CORBA::Object::_duplicate (forward_reference);
  this->is_forwarded_ = true;
}

ACE_INLINE CORBA::Object_ptr
TAO_ServerRequest::forward_location ()
{
  return CORBA::Object::_duplicate (this->forward_location_.in ());
}

ACE_INLINE bool
TAO_ServerRequest::is_forwarded () const
{
  return this->is_forwarded_;
}

ACE_INLINE GIOP::ReplyStatusType
TAO_ServerRequest::reply_status ()
{
  return this->reply_status_;
}

ACE_INLINE void
TAO_ServerRequest::reply_status (GIOP::ReplyStatusType reply_status)
{
  this->reply_status_ = reply_status;
}

ACE_INLINE void
TAO_ServerRequest::is_dsi ()
{
  this->is_dsi_ = true;
}

ACE_INLINE TAO_Operation_Details const *
TAO_ServerRequest::operation_details () const
{
  return this->operation_details_;
}

ACE_INLINE void
TAO_ServerRequest::dsi_nvlist_align (ptrdiff_t alignment)
{
  this->dsi_nvlist_align_ = alignment;
}

ACE_INLINE CORBA::Boolean
TAO_ServerRequest::argument_flag ()
{
  return this->argument_flag_;
}

ACE_INLINE void
TAO_ServerRequest::argument_flag (CORBA::Boolean flag)
{
  this->argument_flag_ = flag;
}

ACE_INLINE bool
TAO_ServerRequest::collocated () const
{
  return this->transport_.get () == 0;
}


#if TAO_HAS_INTERCEPTORS == 1
ACE_INLINE size_t &
TAO_ServerRequest::interceptor_count ()
{
  return this->interceptor_count_;
}

ACE_INLINE CORBA::Exception *
TAO_ServerRequest::caught_exception ()
{
  return this->caught_exception_;
}

ACE_INLINE void
TAO_ServerRequest::pi_reply_status (PortableInterceptor::ReplyStatus s)
{
  this->pi_reply_status_ = s;
}

ACE_INLINE PortableInterceptor::ReplyStatus
TAO_ServerRequest::pi_reply_status ()
{
  return this->pi_reply_status_;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

TAO_END_VERSIONED_NAMESPACE_DECL
