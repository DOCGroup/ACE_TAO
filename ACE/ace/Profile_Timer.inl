// -*- C++ -*-
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_sys_resource.h"
#include "ace/Global_Macros.h"

#if defined (ACE_HAS_GETRUSAGE) && !defined (ACE_WIN32)
#  if defined (ACE_HAS_GETRUSAGE)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_Profile_Timer::start ()
{
  ACE_TRACE ("ACE_Profile_Timer::start");
  this->begin_time_ = ACE_OS::gettimeofday ();
  ACE_OS::getrusage (RUSAGE_SELF,
                     &this->begin_usage_);
  return 0;
}

ACE_INLINE int
ACE_Profile_Timer::stop ()
{
  ACE_TRACE ("ACE_Profile_Timer::stop");
  this->last_time_ = this->end_time_;
  this->end_time_ = ACE_OS::gettimeofday ();
  this->last_usage_ = this->end_usage_;
  ACE_OS::getrusage (RUSAGE_SELF,
                     &this->end_usage_);
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#  endif /* ACE_HAS_GETRUSAGE */

#elif defined (ACE_WIN32)

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
ACE_Profile_Timer::~ACE_Profile_Timer ()
{
}

ACE_INLINE int
ACE_Profile_Timer::start ()
{
  ACE_TRACE ("ACE_Profile_Timer::start");
#  if defined (ACE_HAS_GETRUSAGE)
  ACE_OS::getrusage (RUSAGE_SELF,
                     &this->begin_usage_);
#  endif /* ACE_HAS_GETRUSAGE */
  this->timer_.start ();
  return 0;
}

ACE_INLINE int
ACE_Profile_Timer::stop ()
{
  ACE_TRACE ("ACE_Profile_Timer::stop");
  this->timer_.stop ();
#  if defined (ACE_HAS_GETRUSAGE)
  this->last_usage_ = this->end_usage_;
  ACE_OS::getrusage (RUSAGE_SELF, &this->end_usage_);
#  endif /* ACE_HAS_GETRUSAGE */
  return 0;
}

ACE_END_VERSIONED_NAMESPACE_DECL

#else

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
ACE_Profile_Timer::start ()
{
  ACE_TRACE ("ACE_Profile_Timer::start");
  this->timer_.start ();
  return 0;
}

ACE_INLINE int
ACE_Profile_Timer::stop ()
{
  ACE_TRACE ("ACE_Profile_Timer::stop");
  this->timer_.stop ();
  return 0;
}

ACE_INLINE
ACE_Profile_Timer::~ACE_Profile_Timer ()
{
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* defined (ACE_HAS_GETRUSAGE) */
