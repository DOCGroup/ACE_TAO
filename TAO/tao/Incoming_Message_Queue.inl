// -*- C++ -*-
//$Id$
// -*- C++ -*-
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

/************************************************************************/
// Methods  for TAO_Queued_Data
/************************************************************************/

/*static*/
ACE_INLINE void
TAO_Queued_Data::replace_data_block (ACE_Message_Block &mb)
{
  size_t newsize =
    ACE_CDR::total_length (&mb, 0) + ACE_CDR::MAX_ALIGNMENT;

  ACE_Data_Block *db =
    mb.data_block ()->clone_nocopy ();

  if (db->size (newsize) == -1)
    return;

  ACE_Message_Block tmp (db);
  ACE_CDR::mb_align (&tmp);

  tmp.copy (mb.rd_ptr (), mb.length());
  mb.data_block (tmp.data_block ()->duplicate ());

  mb.rd_ptr (tmp.rd_ptr ());
  mb.wr_ptr (tmp.wr_ptr ());

  // Remove the DONT_DELETE flags from mb
  mb.clr_self_flags (ACE_Message_Block::DONT_DELETE);
}
