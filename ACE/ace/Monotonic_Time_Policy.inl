// -*- C++ -*-
#include "ace/OS_NS_sys_time.h"
#include "ace/High_Res_Timer.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE ACE_Time_Value_T<ACE_Monotonic_Time_Policy>
ACE_Monotonic_Time_Policy::operator()() const
{
#if defined (ACE_MONOTONIC_TIME_POLICY) && (ACE_MONOTONIC_TIME_POLICY >= 0)
#  if defined (ACE_WIN32)
  return ACE_Time_Value_T<ACE_Monotonic_Time_Policy> (ACE_High_Res_Timer::gettimeofday_hr ());
#  elif defined (ACE_HAS_CLOCK_GETTIME_MONOTONIC)
  struct timespec ts;

  if (ACE_OS::clock_gettime (CLOCK_MONOTONIC, &ts) == 0)
    return ACE_Time_Value_T<ACE_Monotonic_Time_Policy>(ts);
  else
    return ACE_Time_Value_T<ACE_Monotonic_Time_Policy> (ACE_Time_Value::zero);
#  else
#    error "No ACE_Monotonic_Time_Policy implementation is valid for this platform"
#  endif /* defined(ACE_WIN32) */
#else /* defined (ACE_MONOTONIC_TIME_POLICY) && (ACE_MONOTONIC_TIME_POLICY >= 0) */
  return ACE_Time_Value_T<ACE_Monotonic_Time_Policy> (ACE_Time_Value::zero);
#endif
}

ACE_INLINE void
ACE_Monotonic_Time_Policy::set_gettimeofday (ACE_Time_Value (*)(void))
{
}

ACE_END_VERSIONED_NAMESPACE_DECL
