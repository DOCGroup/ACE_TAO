/* -*- C++ -*- */
// $Id$

// Message_Block.i

ACE_INLINE ACE_Data_Block *
ACE_Message_Block::data_block (void) const
{
  ACE_TRACE ("ACE_Data_Block::data_block");
  return this->data_block_;
}

ACE_INLINE char *
ACE_Data_Block::base (void) const
{ 
  ACE_TRACE ("ACE_Data_Block::base");
  return this->base_;
}

ACE_INLINE size_t
ACE_Data_Block::size (void) const
{ 
  ACE_TRACE ("ACE_Data_Block::size");
  return this->cur_size_;
}

#if 0
ACE_INLINE void
ACE_Data_Block::base (char *msg_data, 
		      size_t msg_length, 
		      Message_Flags msg_flags)
{
  this->max_size_ = msg_length;
  this->cur_size_ = msg_length;
  this->base_ = msg_data;
  this->flags_ = msg_flags;
}
#endif

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Data_Block::set_flags (ACE_Message_Block::Message_Flags more_flags)
{						
  ACE_TRACE ("ACE_Data_Block::set_flags");
  // Later we might mask more_glags so that user can't change internal
  // ones: more_flags &= ~(USER_FLAGS -1).
  return ACE_SET_BITS (this->flags_, more_flags);
}

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Data_Block::clr_flags (ACE_Message_Block::Message_Flags less_flags)
{						
  ACE_TRACE ("ACE_Data_Block::clr_flags");
  // Later we might mask more_flags so that user can't change internal
  // ones: less_flags &= ~(USER_FLAGS -1).
  return ACE_CLR_BITS (this->flags_, less_flags);
}

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Data_Block::flags (void) const
{
  ACE_TRACE ("ACE_Data_Block::flags");
   return this->flags_;
}

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Message_Block::set_flags (ACE_Message_Block::Message_Flags more_flags)
{						
  ACE_TRACE ("ACE_Message_Block::set_flags");
  return this->data_block ()->set_flags (more_flags);
}

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Message_Block::clr_flags (ACE_Message_Block::Message_Flags less_flags)
{						
  ACE_TRACE ("ACE_Message_Block::clr_flags");
  return this->data_block ()->clr_flags (less_flags);
}

ACE_INLINE ACE_Message_Block::Message_Flags 
ACE_Message_Block::flags (void) const
{
  ACE_TRACE ("ACE_Message_Block::flags");
   return this->data_block ()->flags ();
}

// Return the length of the "active" portion of the message.

ACE_INLINE size_t
ACE_Message_Block::length (void) const
{
  ACE_TRACE ("ACE_Message_Block::length");
  return this->wr_ptr_ - this->rd_ptr_;
}

// Sets the length of the "active" portion of the message.  This is
// defined as the offset from RD_PTR to WR_PTR.

ACE_INLINE void
ACE_Message_Block::length (size_t len)
{
  ACE_TRACE ("ACE_Message_Block::length");
  this->wr_ptr_ = this->rd_ptr_ + len;
}

// Return the length of the potential size of the message.

ACE_INLINE size_t
ACE_Message_Block::size (void) const
{
  ACE_TRACE ("ACE_Message_Block::size");
  return this->data_block ()->size ();
}

ACE_INLINE ACE_Message_Block::ACE_Message_Type
ACE_Data_Block::msg_type (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_type");
  return this->type_;
}

ACE_INLINE void
ACE_Data_Block::msg_type (ACE_Message_Block::ACE_Message_Type t)
{
  ACE_TRACE ("ACE_Message_Block::msg_type");
  this->type_ = t;
}

ACE_INLINE ACE_Message_Block::ACE_Message_Type
ACE_Message_Block::msg_type (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_type");
  return this->data_block ()->msg_type ();
}

ACE_INLINE void
ACE_Message_Block::msg_type (ACE_Message_Block::ACE_Message_Type t)
{
  ACE_TRACE ("ACE_Message_Block::msg_type");
  this->data_block ()->msg_type (t);
}

ACE_INLINE ACE_Message_Block::ACE_Message_Type
ACE_Message_Block::msg_class (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_class");

  if (this->msg_type () < ACE_Message_Block::MB_PRIORITY)
    return ACE_Message_Block::MB_NORMAL;
  else if (this->msg_type () < ACE_Message_Block::MB_USER)
    return ACE_Message_Block::MB_PRIORITY;
  else
    return ACE_Message_Block::MB_USER;
}

ACE_INLINE int
ACE_Message_Block::is_data_msg (void) const
{
  ACE_TRACE ("ACE_Message_Block::is_data_msg");
  ACE_Message_Type mt = this->msg_type ();
  return
    mt == ACE_Message_Block::MB_DATA
    || mt == ACE_Message_Block::MB_PROTO
    || mt == ACE_Message_Block::MB_PCPROTO;
}

ACE_INLINE u_long
ACE_Message_Block::msg_priority (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_priority");
  return this->priority_;
}

ACE_INLINE void
ACE_Message_Block::msg_priority (u_long pri)
{
  ACE_TRACE ("ACE_Message_Block::msg_priority");
  this->priority_ = pri;
}

ACE_INLINE char *
ACE_Message_Block::base (void) const
{
  ACE_TRACE ("ACE_Message_Block::base");
  return this->data_block ()->base ();
}

#if 0
ACE_INLINE void
ACE_Message_Block::base (char *msg_data, 
			 size_t msg_length, 
			 Message_Flags msg_flags)
{
  ACE_TRACE ("ACE_Message_Block::base");
  this->rd_ptr_ = msg_data;
  this->wr_ptr_ = msg_data;
  this->data_block ()->base (msg_data, msg_lenth, msg_flags);
}
#endif

ACE_INLINE char *
ACE_Message_Block::rd_ptr (void)
{
  ACE_TRACE ("ACE_Message_Block::rd_ptr");
  return this->rd_ptr_;
}

ACE_INLINE void
ACE_Message_Block::wr_ptr (char *new_ptr)
{
  ACE_TRACE ("ACE_Message_Block::wr_ptr");
  this->wr_ptr_ = new_ptr;
}

// Return a pointer to 1 past the end of the data buffer.

ACE_INLINE char *
ACE_Data_Block::end (void) const
{
  ACE_TRACE ("ACE_Data_Block::end");
  return this->base_ + this->max_size_;
}

ACE_INLINE char *
ACE_Message_Block::end (void) const
{
  ACE_TRACE ("ACE_Message_Block::end");
  return this->data_block ()->end ();
}


ACE_INLINE void
ACE_Message_Block::rd_ptr (char *new_ptr)
{
  ACE_TRACE ("ACE_Message_Block::rd_ptr");
  this->rd_ptr_ = new_ptr;
}

ACE_INLINE void
ACE_Message_Block::rd_ptr (size_t n)
{
  ACE_TRACE ("ACE_Message_Block::rd_ptr");
  this->rd_ptr_ += n;
}

ACE_INLINE char *
ACE_Message_Block::wr_ptr (void)
{
  ACE_TRACE ("ACE_Message_Block::wr_ptr");
  return this->wr_ptr_;
}

ACE_INLINE void
ACE_Message_Block::wr_ptr (size_t n)
{
  ACE_TRACE ("ACE_Message_Block::wr_ptr");
  this->wr_ptr_ += n;
}

ACE_INLINE void		
ACE_Message_Block::cont (ACE_Message_Block *cont_msg)
{
  ACE_TRACE ("ACE_Message_Block::cont");
  this->cont_ = cont_msg;
}

ACE_INLINE ACE_Message_Block *
ACE_Message_Block::cont (void) const
{
  ACE_TRACE ("ACE_Message_Block::cont");
  return this->cont_;
}

ACE_INLINE void		
ACE_Message_Block::next (ACE_Message_Block *next_msg) 
{
  ACE_TRACE ("ACE_Message_Block::next");
  this->next_ = next_msg;
}

ACE_INLINE ACE_Message_Block *
ACE_Message_Block::next (void) const
{
  ACE_TRACE ("ACE_Message_Block::next");
  return this->next_;
}

ACE_INLINE void		
ACE_Message_Block::prev (ACE_Message_Block *next_msg) 
{
  ACE_TRACE ("ACE_Message_Block::prev");
  this->prev_ = next_msg;
}

ACE_INLINE ACE_Message_Block *
ACE_Message_Block::prev (void) const
{
  ACE_TRACE ("ACE_Message_Block::prev");
  return this->prev_;
}

ACE_INLINE ACE_Lock *
ACE_Data_Block::locking_strategy (void) 
{
  ACE_TRACE ("ACE_Data_Block::locking_strategy");
  return this->locking_strategy_;
}

ACE_INLINE ACE_Lock *
ACE_Data_Block::locking_strategy (ACE_Lock *nls)
{
  ACE_TRACE ("ACE_Data_Block::locking_strategy");
  ACE_Lock *ols = this->locking_strategy_;

  this->locking_strategy_ = nls;
  return ols;
}

ACE_INLINE ACE_Lock *
ACE_Message_Block::locking_strategy (void) 
{
  ACE_TRACE ("ACE_Message_Block::locking_strategy");
  return this->data_block ()->locking_strategy ();
}

ACE_INLINE ACE_Lock *
ACE_Message_Block::locking_strategy (ACE_Lock *nls)
{
  ACE_TRACE ("ACE_Message_Block::locking_strategy");
  ACE_Lock *ols = this->data_block ()->locking_strategy ();
  this->data_block ()->locking_strategy (nls);
  return ols;
}


