/* -*- C++ -*- */
// $Id$

// Activation_Queue.i

ACE_INLINE int 
ACE_Activation_Queue::method_count (void) const 
{
  return queue_->message_count ();
}

ACE_INLINE int 
ACE_Activation_Queue::is_full (void) const 
{
  return queue_->is_full ();
} 

ACE_INLINE int 
ACE_Activation_Queue::is_empty (void) const 
{
  return queue_->is_empty ();
}
