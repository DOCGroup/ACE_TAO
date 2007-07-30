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

ACE_INLINE
TAO_Queued_Data::TAO_Queued_Data (ACE_Allocator *alloc)
  : msg_block_ (0),
    missing_data_ (0),
    major_version_ (0),
    minor_version_ (0),
    byte_order_ (0),
    more_fragments_ (false),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0),
    compressed_ (0),
    allocator_ (alloc)
{
}

ACE_INLINE
TAO_Queued_Data::TAO_Queued_Data (ACE_Message_Block *mb,
                                  ACE_Allocator *alloc)
  : msg_block_ (mb),
    missing_data_ (0),
    major_version_ (0),
    minor_version_ (0),
    byte_order_ (0),
    more_fragments_ (false),
    msg_type_ (TAO_PLUGGABLE_MESSAGE_MESSAGERROR),
    next_ (0),
    compressed_ (0),
    allocator_ (alloc)
{
}

ACE_INLINE
TAO_Queued_Data::TAO_Queued_Data (const TAO_Queued_Data &qd)
  : msg_block_ (qd.msg_block_->duplicate ()),
    missing_data_ (qd.missing_data_),
    major_version_ (qd.major_version_),
    minor_version_ (qd.minor_version_),
    byte_order_ (qd.byte_order_),
    more_fragments_ (qd.more_fragments_),
    msg_type_ (qd.msg_type_),
    next_ (0),
    compressed_ (qd.compressed_),
    allocator_ (qd.allocator_)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
