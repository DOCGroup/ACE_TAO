// $Id$

ACE_INLINE
TAO_EC_Queue::
    TAO_EC_Queue (size_t high_water_mark,
                  size_t low_water_mark,
                  ACE_Notification_Strategy *ns)
      :  ACE_Message_Queue<ACE_SYNCH> (high_water_mark,
                                       low_water_mark,
                                       ns)
{
}

/// @todo The high water mark and low water mark shouldn't be
/// hardcoded, check http://ace.cs.wustl.edu/bugzilla/show_bug.cgi?id=565
ACE_INLINE
TAO_EC_Dispatching_Task::
TAO_EC_Dispatching_Task (ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager),
     allocator_ (0),
     the_queue_ (16384, 16) // @@
{
  this->msg_queue (&this->the_queue_);
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
  :  ACE_Message_Block (data_block,
                        0,
                        mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Shutdown_Task_Command::
    TAO_EC_Shutdown_Task_Command (ACE_Allocator *mb_allocator)
  :  TAO_EC_Dispatch_Command (mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_EC_Push_Command::TAO_EC_Push_Command (
     TAO_EC_ProxyPushSupplier* proxy,
     RtecEventComm::PushConsumer_ptr consumer,
     RtecEventComm::EventSet& event,
     ACE_Data_Block* data_block,
     ACE_Allocator *mb_allocator)
  :  TAO_EC_Dispatch_Command (data_block, mb_allocator),
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
