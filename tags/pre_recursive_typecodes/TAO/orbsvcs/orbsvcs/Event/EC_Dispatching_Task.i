// $Id$

ACE_INLINE
TAO_EC_Dispatching_Task::
TAO_EC_Dispatching_Task (ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Dispatch_Command::TAO_EC_Dispatch_Command (ACE_Allocator *mb_allocator)
  :  ACE_Message_Block (mb_allocator)
{
}

ACE_INLINE
TAO_EC_Dispatch_Command::TAO_EC_Dispatch_Command (ACE_Data_Block *data_block,
                                                  ACE_Allocator *mb_allocator)
  :  ACE_Message_Block (data_block, mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Shutdown_Command::TAO_EC_Shutdown_Command (ACE_Allocator *mb_allocator)
  :  TAO_EC_Dispatch_Command (mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Push_Command::TAO_EC_Push_Command (TAO_EC_ProxyPushSupplier* proxy,
                                          RtecEventComm::EventSet& event,
                                          ACE_Data_Block* data_block,
                                          ACE_Allocator *mb_allocator)
  :  TAO_EC_Dispatch_Command (data_block, mb_allocator),
     proxy_ (proxy)
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
