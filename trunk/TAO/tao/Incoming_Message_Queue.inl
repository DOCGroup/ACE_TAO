// -*- C++ -*-
//$Id$
/************************************************************************/
// Methods for TAO_Queued_Data
/************************************************************************/
/*static*/
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

/*static*/
ACE_INLINE void
TAO_Queued_Data::release (TAO_Queued_Data *qd)
{
  ACE_Message_Block::release (qd->msg_block_);

  // @@TODO: Use the global pool for releasing..
  delete qd;
}


ACE_INLINE TAO_Queued_Data *
TAO_Queued_Data::duplicate (TAO_Queued_Data &sqd)
{
  // Check to see if the underlying block is on the stack. If not it
  // is fine. If the datablock is on stack, try to make a copy of that
  // befor doing a duplicate.
  // @@ todo: Theoretically this should be within the Message Block,
  // but we dont have much scope to do this in that mess. Probably in
  // the next stage of MB rewrite we should be okay
  ACE_Message_Block::Message_Flags fl =
    sqd.msg_block_->self_flags ();

  if (ACE_BIT_ENABLED (fl,
                       ACE_Message_Block::DONT_DELETE))
    (void) TAO_Queued_Data::replace_data_block (*sqd.msg_block_);


  // @@TODO: Use the pool for allocation...
  TAO_Queued_Data *qd = 0;
  ACE_NEW_RETURN (qd,
                  TAO_Queued_Data (sqd),
                  0);

  return qd;
}

/************************************************************************/
// Methods for TAO_Incoming_Message_Queue
/************************************************************************/

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
      this->queued_data_->next_->missing_data_ == 0 &&
      this->queued_data_->next_->more_fragments_ == 0)
    return 1;

  return 0;
}

ACE_INLINE int
TAO_Incoming_Message_Queue::is_tail_fragmented (void)
{
  if (this->size_ == 0)
    return 0;

  if (this->size_  &&
      this->queued_data_->more_fragments_ == 1)
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
