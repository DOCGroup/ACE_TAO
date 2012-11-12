// $Id$

#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"

class Marker
{
 public:

  void accumulate_into (ACE_Throughput_Stats &throughput,
                        int which_method) const;
  // Accumulate the throughput statistics into <throughput>

  void dump_stats (const ACE_TCHAR* msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf,
                   int which_method);
  // Print stats

  void sample (ACE_hrtime_t throughput_diff,
               ACE_hrtime_t latency_diff,
               int which_method);
  // get the sample.

 private:
  ACE_Throughput_Stats throughput_method_ready_;
  // Keep throughput statistics on a per-thread basis
  ACE_Throughput_Stats throughput_method_authenticate_;
  // Keep throughput statistics on a per-thread basis
  ACE_Throughput_Stats throughput_method_update_records_;
  // Keep throughput statistics on a per-thread basis
};
