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

ACE_INLINE CORBA::Octet
TAO_Queued_Data::major_version (void) const
{
  return this->major_version_;
}

ACE_INLINE CORBA::Octet
TAO_Queued_Data::minor_version (void) const
{
  return this->minor_version_;
}

ACE_INLINE CORBA::Octet
TAO_Queued_Data::byte_order (void) const
{
  return this->byte_order_;
}

ACE_INLINE CORBA::Octet
TAO_Queued_Data::more_fragments (void) const
{
  return this->more_fragments_;
}

ACE_INLINE TAO_Pluggable_Message_Type
TAO_Queued_Data::msg_type (void) const
{
  return this->msg_type_;
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
TAO_Queued_Data::set_state (const TAO_GIOP_Message_State& state)
{
  this->byte_order_     = state.byte_order ();
  this->major_version_  = state.giop_version ().major;
  this->minor_version_  = state.giop_version ().minor;
  this->more_fragments_ = state.more_fragments ();
  this->msg_type_       = state.message_type ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
