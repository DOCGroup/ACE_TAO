// $Id$

ACE_INLINE
TAO_CEC_Dispatching_Task::
TAO_CEC_Dispatching_Task (ACE_Thread_Manager* thr_manager)
  :  ACE_Task<ACE_SYNCH> (thr_manager),
     allocator_ (0)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Dispatch_Command::TAO_CEC_Dispatch_Command (ACE_Allocator *mb_allocator)
  :  ACE_Message_Block (mb_allocator)
{
}

ACE_INLINE
TAO_CEC_Dispatch_Command::TAO_CEC_Dispatch_Command (ACE_Data_Block *data_block,
                                                  ACE_Allocator *mb_allocator)
  :  ACE_Message_Block (data_block, 0, mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Shutdown_Task_Command::
    TAO_CEC_Shutdown_Task_Command (ACE_Allocator *mb_allocator)
  :  TAO_CEC_Dispatch_Command (mb_allocator)
{
}

// ****************************************************************

ACE_INLINE
TAO_CEC_Push_Command::TAO_CEC_Push_Command (TAO_CEC_ProxyPushSupplier* proxy,
                                            CORBA::Any& event,
                                            ACE_Data_Block* data_block,
                                            ACE_Allocator *mb_allocator)
  :  TAO_CEC_Dispatch_Command (data_block, mb_allocator),
     proxy_ (proxy)
{
  //
  // Efficient copy, steal the buffer from <event>
  // We cannot do this on the initialization because get_buffer()
  // could get called first, effectively setting maximum() and
  // length() to 0!
  //
  // @@ TODO
  this->event_ = event;

  this->proxy_->_incr_refcnt ();
}
