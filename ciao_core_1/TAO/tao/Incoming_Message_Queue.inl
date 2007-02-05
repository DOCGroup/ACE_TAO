// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/************************************************************************/
// Methods for TAO_Incoming_Message_Queue
/************************************************************************/
ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void) const
{
  return this->size_;
}

/************************************************************************/
// Methods  for TAO_Queued_Data
/************************************************************************/

/*static*/
ACE_INLINE void
TAO_Queued_Data::replace_data_block (ACE_Message_Block &mb)
{
  size_t const newsize =
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

TAO_END_VERSIONED_NAMESPACE_DECL
