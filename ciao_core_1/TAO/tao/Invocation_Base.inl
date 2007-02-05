// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE TAO_ORB_Core *
  Invocation_Base::orb_core (void) const
  {
    return this->orb_core_;
  }

  ACE_INLINE TAO_Stub *
  Invocation_Base::stub (void) const
  {
    return this->stub_;
  }

  ACE_INLINE void
  Invocation_Base::forwarded_reference (CORBA::Object_ptr o)
  {
    this->forwarded_to_ = CORBA::Object::_duplicate (o);
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::forwarded_reference (void)
  {
    return CORBA::Object::_duplicate (this->forwarded_to_.in ());
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::steal_forwarded_reference (void)
  {
    return this->forwarded_to_._retn ();
  }

  ACE_INLINE bool
  Invocation_Base::is_forwarded (void) const
  {
    return (this->forwarded_to_.in () != 0);
  }

  ACE_INLINE CORBA::Boolean
  Invocation_Base::response_expected (void) const
  {
    return this->response_expected_;
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::target (void) const
  {
    return this->otarget_;
  }

  ACE_INLINE CORBA::Object_ptr
  Invocation_Base::effective_target (void) const
  {
    return this->target_;
  }

  ACE_INLINE TAO_Operation_Details &
  Invocation_Base::operation_details (void)
  {
    return this->details_;
  }

#if TAO_HAS_INTERCEPTORS == 1
  ACE_INLINE size_t &
  Invocation_Base::stack_size  (void)
  {
    return this->stack_size_;
  }

  ACE_INLINE CORBA::Exception *
  Invocation_Base::caught_exception (void)
  {
    return this->caught_exception_;
  }

  ACE_INLINE TAO::Invocation_Status
  Invocation_Base::invoke_status (void) const
  {
    return this->invoke_status_;
  }

  ACE_INLINE bool
  Invocation_Base::is_remote_request (void) const
  {
    return this->is_remote_request_;
  }
#endif /* TAO_HAS_INTERCEPTORS */
}

TAO_END_VERSIONED_NAMESPACE_DECL
