// $Id$

#ifndef SEGMENT_TIMERS_H
#define SEGMENT_TIMERS_H

#include "ace/High_Res_Timer.h"
#include "ace/Log_Msg.h"
#include "Segment_Timer_Export.h"

#define MAX_READINGS 500
class Segment_Timer_Export Segment_Timer : public ACE_High_Res_Timer
{
  ACE_hrtime_t recorded_times_[MAX_READINGS];
  int          total_recorded_;
  static bool  dump_flag_;
      
public:
  ACE_INLINE Segment_Timer ()
    :total_recorded_(0)
    {}

  ACE_INLINE void start_timer (void)
  {
    this->start ();
  }
  ACE_INLINE void stop_timer (void)
  {
    this->stop ();
    this->elapsed_time (recorded_times_[total_recorded_]);
    ++total_recorded_;
  }
  void dump (int index); 
  static void set_dump_flag(bool flag) { dump_flag_ = flag;}
  static bool get_dump_flag(void) { return dump_flag_;}
};

enum {CREATE_HOME_TIMER, CREATE_CONTAINER_TIMER, CREATE_COMPONENT_TIMER,
      REGISTER_COMPONENT_TIMER, CREATE_CONNECTION_TIMER, TOTAL_ASSEMBLY_TIMER, CREATE_COMPSERVER_TIMER, MAX_TIMERS};

extern Segment_Timer_Export Segment_Timer segment_timers[MAX_TIMERS];

#endif
