// $Id$

ACE_INLINE
ACE_RMCast_Control_Splitter::ACE_RMCast_Control_Splitter (void)
  : ACE_RMCast_Module ()
  , control_module_ (0)
{
}

ACE_INLINE void
ACE_RMCast_Control_Splitter::control_module (ACE_RMCast_Module *module)
{
  this->control_module_ = module;
}


ACE_INLINE ACE_RMCast_Module *
ACE_RMCast_Control_Splitter::control_module (void) const
{
  return this->control_module_;
}
