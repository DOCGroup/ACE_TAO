// -*- C++ -*-
//$Id$
ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void)
{
  return this->size_;
}

ACE_INLINE int
TAO_Incoming_Message_Queue::is_complete_message (void)
{
  if (this->size_ != 0 &&
      this->queued_data_->missing_data_ == 0)
    return 0;

  return 1;
}

ACE_INLINE char *
TAO_Incoming_Message_Queue::wr_ptr (void) const
{
  return this->queued_data_->msg_block_->wr_ptr ();
}

ACE_INLINE size_t
TAO_Incoming_Message_Queue::missing_data (void) const
{
  if (this->size_ != 0)
    return this->queued_data_->missing_data_;

  return 0;
}



ACE_INLINE TAO_Incoming_Message_Queue::TAO_Queued_Data *
TAO_Incoming_Message_Queue::get_node (void)
{
  // @@TODO: Use the global pool for allocationg...
  TAO_Queued_Data *qd = 0;
  ACE_NEW_RETURN (qd,
                  TAO_Queued_Data,
                  0);

  return qd;
}

ACE_INLINE
TAO_Incoming_Message_Queue::TAO_Queued_Data::TAO_Queued_Data (void)
  : msg_block_ (0),
    missing_data_ (0),
    byte_order_ (0),
    next_ (0)
{
}
