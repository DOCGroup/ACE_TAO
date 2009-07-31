// $Id$

inline void
operator<<= (::DDS::Time_t &ddsstatus, const ::DDS_Time_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator<<= (::DDS_Time_t &ddsstatus, const ::DDS::Time_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator>>= (const ::DDS_Time_t &status, ::DDS::Time_t & ddsstatus)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

