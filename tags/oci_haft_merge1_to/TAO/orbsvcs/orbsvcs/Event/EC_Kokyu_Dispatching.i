// $Id$
// ****************************************************************

ACE_INLINE
TAO_EC_Kokyu_Shutdown_Command::
        TAO_EC_Kokyu_Shutdown_Command (void)
  :  Kokyu::Dispatch_Command ()
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Kokyu_Push_Command::TAO_EC_Kokyu_Push_Command (
     TAO_EC_ProxyPushSupplier* proxy,
     RtecEventComm::PushConsumer_ptr consumer,
     RtecEventComm::EventSet& event)
  :  Kokyu::Dispatch_Command (),
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
