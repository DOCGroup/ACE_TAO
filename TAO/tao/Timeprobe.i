//
// $Id$
//

ACE_INLINE
ACE_Timeprobe::ACE_Timeprobe ()
  : current_slot_ (0)
{
}

ACE_INLINE
ACE_Timeprobe::~ACE_Timeprobe ()
{
}

ACE_INLINE void
ACE_Timeprobe::destroy ()
{
  delete instance_;
  instance_ = 0;
}

ACE_INLINE void
ACE_Timeprobe::reset()
{
  current_slot_ = 0;
}
