// $Id$

inline void
operator<<= (::DDS::Duration_t &ddsstatus, const ::DDS_Duration_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator<<= (::DDS_Duration_t &ddsstatus, const ::DDS::Duration_t & status)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

inline void
operator>>= (const ::DDS_Duration_t &status, ::DDS::Duration_t & ddsstatus)
{
  ddsstatus.sec = status.sec;
  ddsstatus.nanosec = status.nanosec;
}

