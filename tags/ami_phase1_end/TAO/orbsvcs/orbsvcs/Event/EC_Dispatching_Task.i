// $Id$

ACE_INLINE
TAO_EC_Dispatching_Task::
TAO_EC_Dispatching_Task (ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Shutdown_Command::TAO_EC_Shutdown_Command (void)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Push_Command::TAO_EC_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                                          RtecEventComm::EventSet& event)
  :  proxy_ (proxy)
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
}
