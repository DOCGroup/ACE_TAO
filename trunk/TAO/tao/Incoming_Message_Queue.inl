// -*- C++ -*-
//$Id$
/************************************************************************/
// Methods for TAO_Queued_Data
/************************************************************************/
ACE_INLINE TAO_Queued_Data *
TAO_Queued_Data::get_queued_data (void)
{
  // @@TODO: Use the global pool for allocationg...
  TAO_Queued_Data *qd = 0;
  ACE_NEW_RETURN (qd,
                  TAO_Queued_Data,
                  0);

  return qd;
}

ACE_INLINE void
TAO_Queued_Data::release (TAO_Queued_Data *qd)
{
  ACE_Message_Block::release (qd->msg_block_);

  // @@TODO: Use the global pool for releasing..
  delete qd;
}

ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void)
{
  return this->size_;
}

ACE_INLINE int
TAO_Incoming_Message_Queue::is_tail_complete (void)
{
  // If the size is 0 return -1
  if (this->size_ == 0)
    return -1;

  if (this->size_ &&
      this->queued_data_->missing_data_ == 0)
    return 1;

  return 0;
}

ACE_INLINE int
TAO_Incoming_Message_Queue::is_head_complete (void)
{
  if (this->size_ == 0)
    return -1;

  if (this->size_  &&
      this->queued_data_->next_->missing_data_ == 0)
    return 1;

  return 0;
}

ACE_INLINE size_t
TAO_Incoming_Message_Queue::missing_data_tail (void) const
{
  if (this->size_ != 0)
    return this->queued_data_->missing_data_;

  return 0;
}



ACE_INLINE TAO_Queued_Data *
TAO_Incoming_Message_Queue::get_node (void)
{
  return TAO_Queued_Data::get_queued_data ();
}
