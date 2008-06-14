// -*- C++ -*-
//
// $Id$

#include "ace/Time_Value.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE TimeBase::TimeT
ORBSVCS_Time::zero ()
{
  return 0;
}

ACE_INLINE void
ORBSVCS_Time::TimeT_to_hrtime (ACE_hrtime_t &lhs,
                               const TimeBase::TimeT& rhs)
{
  lhs = rhs;
  lhs *= 100;
}

ACE_INLINE void
ORBSVCS_Time::hrtime_to_TimeT (TimeBase::TimeT& lhs,
                               ACE_hrtime_t rhs)
{
  ACE_UINT64 t = rhs / 100; // convert to 100ns
  lhs = t;
}

ACE_INLINE ACE_hrtime_t
ORBSVCS_Time::to_hrtime (const TimeBase::TimeT& t)
{
  ACE_hrtime_t r;
  ORBSVCS_Time::TimeT_to_hrtime (r, t);
  return r;
}

ACE_INLINE void
ORBSVCS_Time::Time_Value_to_TimeT (TimeBase::TimeT& lhs,
                                   const ACE_Time_Value& rhs)
{
  ACE_hrtime_t t =
    static_cast<ACE_hrtime_t> (rhs.sec ()) * ACE_U_ONE_SECOND_IN_NSECS +
    static_cast<ACE_hrtime_t> (rhs.usec ()) * 1000u;

  ORBSVCS_Time::hrtime_to_TimeT (lhs, t);
}

ACE_INLINE void
ORBSVCS_Time::TimeT_to_Time_Value (ACE_Time_Value& lhs,
                                   const TimeBase::TimeT& rhs)
{
  ACE_hrtime_t t;

  ORBSVCS_Time::TimeT_to_hrtime (t, rhs);

  lhs.set (static_cast<ACE_UINT32> (t / ACE_U_ONE_SECOND_IN_NSECS),
           static_cast<ACE_UINT32> ((t % ACE_U_ONE_SECOND_IN_NSECS) / 1000));
}

ACE_INLINE ACE_Time_Value
ORBSVCS_Time::to_Time_Value (const TimeBase::TimeT& t)
{
  ACE_Time_Value r;
  ORBSVCS_Time::TimeT_to_Time_Value (r, t);
  return r;
}

ACE_INLINE TimeBase::TimeT
ORBSVCS_Time::to_TimeT (const ACE_Time_Value& t)
{
   TimeBase::TimeT r;
   ORBSVCS_Time::Time_Value_to_TimeT (r, t);
   return r;
}

ACE_INLINE void
ORBSVCS_Time::Absolute_Time_Value_to_TimeT (TimeBase::TimeT& lhs,
	                const ACE_Time_Value& rhs)
{
  ACE_hrtime_t t =
    static_cast<ACE_hrtime_t> (rhs.sec ()) * ACE_U_ONE_SECOND_IN_NSECS +
    static_cast<ACE_hrtime_t> (rhs.usec ()) * 1000u;

  t += Time_Base_Offset;
  ORBSVCS_Time::hrtime_to_TimeT (lhs, t);
}

ACE_INLINE void
ORBSVCS_Time::Absolute_TimeT_to_Time_Value (ACE_Time_Value& lhs,
	                const TimeBase::TimeT& rhs)
{
  ACE_hrtime_t t;

  ORBSVCS_Time::TimeT_to_hrtime (t, rhs);
  t -= Time_Base_Offset;

  lhs.set(static_cast<ACE_UINT32> (t / ACE_U_ONE_SECOND_IN_NSECS),
    static_cast<ACE_UINT32> ((t % ACE_U_ONE_SECOND_IN_NSECS) / 1000));
}

ACE_INLINE ACE_Time_Value
ORBSVCS_Time::to_Absolute_Time_Value (const TimeBase::TimeT& t)
{
  ACE_Time_Value r;
  ORBSVCS_Time::Absolute_TimeT_to_Time_Value (r, t);
  return r;
}

ACE_INLINE TimeBase::TimeT
ORBSVCS_Time::to_Absolute_TimeT (const ACE_Time_Value& t)
{
  TimeBase::TimeT r;
  ORBSVCS_Time::Absolute_Time_Value_to_TimeT (r, t);
  return r;
}

TAO_END_VERSIONED_NAMESPACE_DECL
