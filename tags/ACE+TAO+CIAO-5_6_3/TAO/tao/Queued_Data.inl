// -*- C++ -*-
//
//$Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    state_ (),
    next_ (0),
    allocator_ (alloc)
{
}

ACE_INLINE
TAO_Queued_Data::TAO_Queued_Data (ACE_Message_Block *mb,
                                  ACE_Allocator *alloc)
  : msg_block_ (mb),
    missing_data_ (0),
    state_ (),
    next_ (0),
    allocator_ (alloc)
{
}

ACE_INLINE
TAO_Queued_Data::TAO_Queued_Data (const TAO_Queued_Data &qd)
  : msg_block_ (qd.msg_block_->duplicate ()),
    missing_data_ (qd.missing_data_),
    state_ (qd.state_),
    next_ (0),
    allocator_ (qd.allocator_)
{
}


ACE_INLINE size_t
TAO_Queued_Data::missing_data (void) const
{
  return this->missing_data_;
}

ACE_INLINE void
TAO_Queued_Data::missing_data (size_t data)
{
  this->missing_data_ = data;
}

ACE_INLINE TAO_GIOP_Message_Version const &
TAO_Queued_Data::giop_version (void) const
{
  return this->state_.giop_version ();
}

ACE_INLINE CORBA::Octet
TAO_Queued_Data::byte_order (void) const
{
  return this->state_.byte_order ();
}

ACE_INLINE CORBA::Octet
TAO_Queued_Data::more_fragments (void) const
{
  return this->state_.more_fragments ();
}

ACE_INLINE GIOP::MsgType
TAO_Queued_Data::msg_type (void) const
{
  return this->state_.message_type ();
}

ACE_INLINE TAO_Queued_Data *
TAO_Queued_Data::next (void) const
{
  return this->next_;
}

ACE_INLINE void
TAO_Queued_Data::next (TAO_Queued_Data* qd)
{
  this->next_ = qd;
}

ACE_INLINE ACE_Message_Block *
TAO_Queued_Data::msg_block (void) const
{
  return this->msg_block_;
}

ACE_INLINE void
TAO_Queued_Data::msg_block (ACE_Message_Block *mb)
{
  this->msg_block_ = mb;
}

ACE_INLINE void
TAO_Queued_Data::state (const TAO_GIOP_Message_State& state)
{
  this->state_ = state;
}

ACE_INLINE const TAO_GIOP_Message_State&
TAO_Queued_Data::state (void) const
{
  return this->state_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
