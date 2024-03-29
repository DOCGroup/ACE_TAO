// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE TAO_Stub *
  Invocation_Base::stub () const
  {
    return this->stub_;
  }

  ACE_INLINE void
  Invocation_Base::forwarded_reference (CORBA::Object_ptr o)
  {
    this->forwarded_to_ = CORBA::Object::_duplicate (o);
    this->reply_status_ = GIOP::LOCATION_FORWARD;
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::forwarded_reference ()
  {
    return CORBA::Object::_duplicate (this->forwarded_to_.in ());
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::steal_forwarded_reference ()
  {
    this->reply_status_ = GIOP::NO_EXCEPTION;
    return this->forwarded_to_._retn ();
  }

  ACE_INLINE CORBA::Boolean
  Invocation_Base::response_expected () const
  {
    return this->response_expected_;
  }

  ACE_INLINE GIOP::ReplyStatusType
  Invocation_Base::reply_status () const
  {
    return this->reply_status_;
  }

  ACE_INLINE void
  Invocation_Base::reply_status (GIOP::ReplyStatusType s)
  {
    this->reply_status_ = s;
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::target () const
  {
    return this->otarget_;
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::effective_target () const
  {
    return this->target_;
  }

  ACE_INLINE TAO_Operation_Details &
  Invocation_Base::operation_details ()
  {
    return this->details_;
  }

#if TAO_HAS_INTERCEPTORS == 1
  ACE_INLINE size_t &
  Invocation_Base::stack_size  ()
  {
    return this->stack_size_;
  }

  ACE_INLINE CORBA::Exception *
  Invocation_Base::caught_exception ()
  {
    return this->caught_exception_;
  }

  ACE_INLINE TAO::Invocation_Status
  Invocation_Base::invoke_status () const
  {
    return this->invoke_status_;
  }

  ACE_INLINE void
  Invocation_Base::invoke_status (Invocation_Status s)
  {
    this->invoke_status_ = s;
  }

  ACE_INLINE bool
  Invocation_Base::is_remote_request () const
  {
    return this->is_remote_request_;
  }
#endif /* TAO_HAS_INTERCEPTORS */
}

TAO_END_VERSIONED_NAMESPACE_DECL
