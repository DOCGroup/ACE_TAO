// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_INLINE
TAO_EC_Kokyu_Shutdown_Command::
TAO_EC_Kokyu_Shutdown_Command (ACE_Allocator*)
  :  Kokyu::Dispatch_Command ()
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Kokyu_Push_Command::TAO_EC_Kokyu_Push_Command (
     TAO_EC_ProxyPushSupplier* proxy,
     RtecEventComm::PushConsumer_ptr consumer,
     RtecEventComm::EventSet& event,
     ACE_Allocator* allocator)
  :  Kokyu::Dispatch_Command (0,allocator),
     proxy_ (proxy),
     consumer_ (RtecEventComm::PushConsumer::_duplicate (consumer))
{
  //
  // Efficient copy, steal the buffer from <event>
  // We cannot do this on the initialization because get_buffer()
  // could get called first, effectively setting maximum() and
  // length() to 0!
  //
  CORBA::ULong maximum = event.maximum ();
  CORBA::ULong length = event.length ();
  RtecEventComm::Event* buffer = event.get_buffer (1);
  event_.replace (maximum, length, buffer, 1);

  this->proxy_->_incr_refcnt ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
