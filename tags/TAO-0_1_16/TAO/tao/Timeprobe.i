// $Id$

ACE_INLINE
ACE_Timeprobe::ACE_Timeprobe (void)
  : current_slot_ (0)
{
}

ACE_INLINE
ACE_Timeprobe::~ACE_Timeprobe (void)
{
}

ACE_INLINE void
ACE_Timeprobe::destroy (void)
{
  delete instance_;
  instance_ = 0;
}

ACE_INLINE void
ACE_Timeprobe::reset (void)
{
  current_slot_ = 0;
}
