// $Id$

ACE_INLINE
ACE_RMCast_Fork::ACE_RMCast_Fork (void)
  : ACE_RMCast_Module ()
  , secondary_ (0)
{
}

ACE_INLINE void
ACE_RMCast_Fork::secondary (ACE_RMCast_Module *module)
{
  this->secondary_ = module;
}


ACE_INLINE ACE_RMCast_Module *
ACE_RMCast_Fork::secondary (void) const
{
  return this->secondary_;
}
