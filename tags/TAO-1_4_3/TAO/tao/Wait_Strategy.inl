//$Id$

ACE_INLINE int
TAO_Wait_Strategy::is_registered (void)
{
  return this->is_registered_;
}

ACE_INLINE void
TAO_Wait_Strategy::is_registered (int flag)
{
  this->is_registered_ = flag;
}

