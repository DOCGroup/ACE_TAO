//$Id$

#include "duration.h"

Duration::Duration ()
{
}


bool
Duration::check (const ::DDS_Duration_t & dds_dur,
                 const ::DDS::Duration_t & dur)
{
  return dds_dur.sec == dur.sec &&
         dds_dur.nanosec == dur.nanosec;
}
