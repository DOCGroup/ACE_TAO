// $Id$

ACE_INLINE void
ORBSVCS_Time::TimeT_to_hrtime (ACE_hrtime_t &lhs,
                               const TimeBase::TimeT& rhs)
{
  lhs = rhs.high;
  lhs <<= 32;
  lhs |= rhs.low;
  lhs *= 100;
}

ACE_INLINE void
ORBSVCS_Time::hrtime_to_TimeT (TimeBase::TimeT& lhs,
                               ACE_hrtime_t rhs)
{
  ACE_UINT64 t = rhs / 100; // convert to 100ns
  ACE_UINT32 mask = ~0L;
  lhs.low = ACE_static_cast(ACE_UINT32, t & mask);
#if defined (ACE_LACKS_LONGLONG_T)
  lhs.high = ACE_static_cast (ACE_UINT32, t.hi ());
#else  /* ! ACE_LACKS_LONGLONG_T */
  lhs.high = ACE_static_cast (ACE_UINT32, t >> 32);
#endif /* ! ACE_LACKS_LONGLONG_T */
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
  ACE_hrtime_t t = rhs.sec () * ACE_ONE_SECOND_IN_NSECS + rhs.usec () * 1000;
  ORBSVCS_Time::hrtime_to_TimeT (lhs, t);
}

ACE_INLINE void
ORBSVCS_Time::TimeT_to_Time_Value (ACE_Time_Value& lhs,
                                   const TimeBase::TimeT& rhs)
{
  ACE_hrtime_t t;

  ORBSVCS_Time::TimeT_to_hrtime (t, rhs);

  lhs.set (ACE_static_cast(ACE_UINT32, t / ACE_ONE_SECOND_IN_NSECS),
           ACE_static_cast(ACE_UINET32, (t % ACE_ONE_SECOND_IN_NSECS) / 1000));
}

ACE_INLINE ACE_Time_Value
ORBSVCS_Time::to_Time_Value (const TimeBase::TimeT& t)
{
  ACE_Time_Value r;
  ORBSVCS_Time::TimeT_to_Time_Value (r, t);
  return r;
}

ACE_INLINE int
operator== (const TimeBase::TimeT &lhs,
            const TimeBase::TimeT &rhs)
{
  return (lhs.low == rhs.low
          && lhs.high == rhs.high);
}

ACE_INLINE int
operator!= (const TimeBase::TimeT &lhs,
            const TimeBase::TimeT &rhs)
{
  return !(lhs == rhs);
}

ACE_INLINE int
operator< (const TimeBase::TimeT &lhs,
           const TimeBase::TimeT &rhs)
{
  return (lhs.high < rhs.high
          || (lhs.high == rhs.high && lhs.low < rhs.low));
}

ACE_INLINE int
operator<= (const TimeBase::TimeT &lhs,
            const TimeBase::TimeT &rhs)
{
  return (lhs.high < rhs.high
          || (lhs.high == rhs.high && lhs.low <= rhs.low));
}

ACE_INLINE int
operator> (const TimeBase::TimeT &lhs,
           const TimeBase::TimeT &rhs)
{
  return !(rhs <= lhs);
}

ACE_INLINE int
operator>= (const TimeBase::TimeT &lhs,
            const TimeBase::TimeT &rhs)
{
  return !(rhs < lhs);
}
