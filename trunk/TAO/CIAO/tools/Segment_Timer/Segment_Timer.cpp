// $Id$

#include "Segment_Timer.h"

Segment_Timer segment_timers[MAX_TIMERS];

char* Static_Config_Timer_Descriptions[] =
  {
    "Create Home",
    "Create Container",
    "Create Component",
    "Register Component",
    "Create Connection",
    "Total Assembly time",
    "Create Comp Server"
  };

bool Segment_Timer::dump_flag_ = false;

void Segment_Timer::dump (int index)
{
  ACE_DEBUG ((LM_DEBUG, "%s\n======================\n",
              Static_Config_Timer_Descriptions[index]));

  for (int i=0; i<total_recorded_; ++i)
    {
      //ACE_Time_Value tv;
      //this->hrtime_to_tv (tv, recorded_times_[i]);
      //ACE_DEBUG ((LM_DEBUG, "%f msec \n", recorded_times_[i]/1000000));
    }
}
