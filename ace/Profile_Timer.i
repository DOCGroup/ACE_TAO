/* -*- C++ -*- */
// $Id$

// Profile_Timer.i

#if defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE)

#if defined (ACE_HAS_PRUSAGE_T)
ACE_INLINE int 
ACE_Profile_Timer::start (void) 
{
  ACE_TRACE ("ACE_Profile_Timer::start");
  return ACE_OS::ioctl (this->proc_handle_, 
			PIOCUSAGE, 
			&this->begin_usage_);
}

ACE_INLINE int 
ACE_Profile_Timer::stop (void) 
{
  ACE_TRACE ("ACE_Profile_Timer::stop");
  this->last_usage_ = this->end_usage_;
  return ACE_OS::ioctl (this->proc_handle_, 
		    PIOCUSAGE, 
		    &this->end_usage_);
}
#elif defined (ACE_HAS_GETRUSAGE) 
ACE_INLINE int 
ACE_Profile_Timer::start (void) 
{
  ACE_TRACE ("ACE_Profile_Timer::start");
  this->begin_time_ = ACE_High_Res_Timer::gettimeofday ();
  ACE_OS::getrusage (RUSAGE_SELF, &this->begin_usage_);
  return 0;
}

ACE_INLINE int 
ACE_Profile_Timer::stop (void) 
{
  ACE_TRACE ("ACE_Profile_Timer::stop");
  this->last_time_ = this->end_time_;
  this->end_time_ = ACE_High_Res_Timer::gettimeofday ();
  this->last_usage_ = this->end_usage_;
  ACE_OS::getrusage (RUSAGE_SELF, &this->end_usage_);
  return 0;
}
#endif /* ACE_HAS_PRUSAGE_T */
#endif /* defined (ACE_HAS_PRUSAGE_T) || defined (ACE_HAS_GETRUSAGE) */
