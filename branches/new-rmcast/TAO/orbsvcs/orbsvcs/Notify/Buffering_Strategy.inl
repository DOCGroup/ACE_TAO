// $Id$

ACE_INLINE void
TAO_Notify_Buffering_Strategy::batch_size (CORBA::Long batch_size)
{
  this->batch_size_ = batch_size;
}

ACE_INLINE CORBA::Long
TAO_Notify_Buffering_Strategy::batch_size (void)
{
  return this->batch_size_;
}

ACE_INLINE void
TAO_Notify_Buffering_Strategy::max_local_queue_length (CORBA::Long length)
{
  this->max_local_queue_length_ = length;
}
