// -*- C++ -*-
//$Id$
ACE_INLINE const ACE_Data_Block *
TAO_Incoming_Message_Queue::current_message (void) const
{

  return this->queued_data_->data_block_;
}

ACE_INLINE const CORBA::Octet
TAO_Incoming_Message_Queue::current_byte_order (void) const
{
  return this->queued_data_->byte_order_;
}


ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void)
{
  return this->size_;
}

ACE_INLINE int
TAO_Incoming_Message_Queue::complete_message (void)
{
  if (this->queued_data_ != 0 &&
      this->queued_data_->next_->missing_data_ == 0)
    return 1;

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
  : data_block_ (0),
    missing_data_ (0),
    byte_order_ (0),
    next_ (0)
{
}
