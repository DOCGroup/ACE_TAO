/* -*- C++ -*- */
// $Id$

// Message_Block.i

ACE_INLINE ACE_Data_Block *
ACE_Message_Block::data_block (void) const
{
  ACE_TRACE ("ACE_Message_Block::data_block");
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

ACE_INLINE ACE_Allocator*
ACE_Data_Block::data_block_allocator (void) const
{
  ACE_TRACE ("ACE_Message_Block::data_block_allocator");
  return this->data_block_allocator_;
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

ACE_INLINE size_t
ACE_Message_Block::space (void) const
{
  ACE_TRACE ("ACE_Message_Block::space");
  return this->size () - this->length ();
}

ACE_INLINE ACE_Message_Block::ACE_Message_Type
ACE_Data_Block::msg_type (void) const
{
  ACE_TRACE ("ACE_Data_Block::msg_type");
  return this->type_;
}

ACE_INLINE void
ACE_Data_Block::msg_type (ACE_Message_Block::ACE_Message_Type t)
{
  ACE_TRACE ("ACE_Data_Block::msg_type");
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

ACE_INLINE const ACE_Time_Value & 
ACE_Message_Block::msg_execution_time (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_execution_time (void)");
  return this->execution_time_;
}


ACE_INLINE void 
ACE_Message_Block::msg_execution_time (const ACE_Time_Value & et)
{
  ACE_TRACE ("ACE_Message_Block::msg_execution_time (const ACE_Time_Value & et)");
  this->execution_time_ = et;
}

ACE_INLINE const ACE_Time_Value & 
ACE_Message_Block::msg_deadline_time (void) const
{
  ACE_TRACE ("ACE_Message_Block::msg_deadline_time (void)");
  return this->deadline_time_;
}

ACE_INLINE void 
ACE_Message_Block::msg_deadline_time (const ACE_Time_Value & dt)
{
  ACE_TRACE ("ACE_Message_Block::msg_deadline_time (const ACE_Time_Value & et)");
  this->deadline_time_ = dt;
}

ACE_INLINE char *
ACE_Message_Block::base (void) const
{
  ACE_TRACE ("ACE_Message_Block::base");
  return this->data_block ()->base ();
}

ACE_INLINE void
ACE_Message_Block::base (char *msg_data,
			 size_t msg_length,
			 Message_Flags msg_flags)
{
  ACE_TRACE ("ACE_Message_Block::base");
  this->rd_ptr_ = 0;
  this->wr_ptr_ = 0;
  this->data_block ()->base (msg_data, msg_length, msg_flags);
}

ACE_INLINE char *
ACE_Message_Block::rd_ptr (void) const
{
  ACE_TRACE ("ACE_Message_Block::rd_ptr");
  return this->base () + this->rd_ptr_;
}

ACE_INLINE void
ACE_Message_Block::wr_ptr (char *new_ptr)
{
  ACE_TRACE ("ACE_Message_Block::wr_ptr");
  this->wr_ptr_ = new_ptr - this->base ();
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
  this->rd_ptr_ = new_ptr - this->base ();
}

ACE_INLINE void
ACE_Message_Block::rd_ptr (size_t n)
{
  ACE_TRACE ("ACE_Message_Block::rd_ptr");
  this->rd_ptr_ += n;
}

ACE_INLINE char *
ACE_Message_Block::wr_ptr (void) const
{
  ACE_TRACE ("ACE_Message_Block::wr_ptr");
  return this->base () + this->wr_ptr_;
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

ACE_INLINE ACE_Allocator *
ACE_Data_Block::allocator_strategy (void) const
{
  ACE_TRACE ("ACE_Data_Block::allocator_strategy");
  return this->allocator_strategy_;
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

ACE_INLINE int
ACE_Data_Block::reference_count (void) const
{
  return reference_count_;
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


////////////////////////////////////////
// class ACE_Dynamic_Message_Strategy //
////////////////////////////////////////

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::static_bit_field_mask (void)
{
  return static_bit_field_mask_;
}
  // get static bit field mask

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::static_bit_field_mask (u_long ul)
{
  static_bit_field_mask_ = ul;
}
  // set static bit field mask

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::static_bit_field_shift (void)
{
  return static_bit_field_shift_;
}
  // get left shift value to make room for static bit field

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::static_bit_field_shift (u_long ul)
{
  static_bit_field_shift_ = ul;
}
  // set left shift value to make room for static bit field

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::dynamic_priority_max (void)
{
  return dynamic_priority_max_;
}
  // get maximum supported priority value

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::dynamic_priority_max (u_long ul)
{
  // pending_shift_ depends on dynamic_priority_max_: for performance
  // reasons, the value in pending_shift_ is (re)calculated only when 
  // dynamic_priority_max_ is initialized or changes, and is stored
  // as a class member rather than being a derived value.
  dynamic_priority_max_ = ul;
  pending_shift_ = ACE_Time_Value (0, ul);
}
  // set maximum supported priority value

ACE_INLINE u_long 
ACE_Dynamic_Message_Strategy::dynamic_priority_offset (void)
{
  return dynamic_priority_offset_;
}
  // get offset for boundary between signed range and unsigned range

ACE_INLINE void 
ACE_Dynamic_Message_Strategy::dynamic_priority_offset (u_long ul)
{


  // max_late_ and min_pending_ depend on dynamic_priority_offset_: for 
  // performance reasons, the values in max_late_ and min_pending_ are
  // (re)calculated only when dynamic_priority_offset_ is initialized
  // or changes, and are stored as a class member rather than being
  // derived each time one of their values is needed.
  dynamic_priority_offset_ = ul;  
  max_late_ = ACE_Time_Value (0, ul - 1);
  min_pending_ = ACE_Time_Value (0, ul);
}
  // set offset for boundary between signed range and unsigned range


ACE_INLINE ACE_Dynamic_Message_Strategy::Priority_Status
ACE_Dynamic_Message_Strategy::priority_status (ACE_Message_Block & mb,
                                               const ACE_Time_Value & tv)
{
  // default the message to have pending priority status
  Priority_Status status = ACE_Dynamic_Message_Strategy::PENDING;

  // start with the passed absolute time as the message's priority, then
  // call the polymorphic hook method to (at least partially) convert 
  // the absolute time and message attributes into the message's priority
  ACE_Time_Value priority (tv);  
  convert_priority (priority, mb);

  // if the priority is negative, the message is pending
  if (priority < ACE_Time_Value::zero)
  {
    // priority for pending messages must be shifted
    // upward above the late priority range
    priority += pending_shift_;
    if (priority < min_pending_)
    {
      priority = min_pending_;
    }
  }
  // otherwise, if the priority is greater than the maximum late
  // priority value that can be represented, it is beyond late 
  else if (priority > max_late_)
  {
    // all messages that are beyond late are assigned lowest priority (zero)
    mb.msg_priority (0);
    return ACE_Dynamic_Message_Strategy::BEYOND_LATE;
  }  
  // otherwise, the message is late, but its priority is correct  
  else
  {
    status = ACE_Dynamic_Message_Strategy::LATE;
  }

  // use (fast) bitwise operators to isolate and replace
  // the dynamic portion of the message's priority
  mb.msg_priority((mb.msg_priority() & static_bit_field_mask_) | 
                  ((priority.usec () + ACE_ONE_SECOND_IN_USECS * priority.sec ()) <<
                   static_bit_field_shift_));

  return status;
}
  // returns the priority status of the message



/////////////////////////////////////////
// class ACE_Deadline_Message_Strategy //
/////////////////////////////////////////

ACE_INLINE void 
ACE_Deadline_Message_Strategy::convert_priority (ACE_Time_Value & priority,
                                                 const ACE_Message_Block & mb)
{
  // Convert absolute time passed in tv to negative time
  // to deadline of mb with respect to that absolute time.
  priority -= mb.msg_deadline_time ();
}
  // dynamic priority conversion function based on time to deadline


///////////////////////////////////////
// class ACE_Laxity_Message_Strategy //
///////////////////////////////////////

ACE_INLINE void 
ACE_Laxity_Message_Strategy::convert_priority (ACE_Time_Value & priority, 
                                               const ACE_Message_Block & mb)
{
  // Convert absolute time passed in tv to negative
  // laxity of mb with respect to that absolute time.
  priority += mb.msg_execution_time ();
  priority -= mb.msg_deadline_time ();
}
  // dynamic priority conversion function based on laxity
