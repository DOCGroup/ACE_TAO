/* -*- C++ -*- */
//$Id$



ACE_INLINE ACE_Recyclable_State
ACE_Recyclable::recycle_state (void) const
{
  return this->recycle_state_;
}

ACE_INLINE void
ACE_Recyclable::recycle_state (ACE_Recyclable_State new_state)
{
  if (this->recycle_state_ == ACE_RECYCLABLE_CLOSED)
    return;

  this->recycle_state_ = new_state;
}
