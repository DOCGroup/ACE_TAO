// -*- C++ -*-
// $Id$
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

  ACE_INLINE void
  Invocation_Base::reply_received (Invocation_Status s)
  {
#if TAO_HAS_INTERCEPTORS == 1
    this->req_info_.reply_status (s);
#endif /*TAO_HAS_INTERCEPTORS*/
    ACE_UNUSED_ARG (s);
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



}
