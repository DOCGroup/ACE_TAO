// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Incoming_Message_Queue::TAO_Incoming_Message_Queue (TAO_ORB_Core *orb_core)
  : last_added_ (0),
    size_ (0),
    orb_core_ (orb_core)
{
}

ACE_INLINE CORBA::ULong
TAO_Incoming_Message_Queue::queue_length (void) const
{
  return this->size_;
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
