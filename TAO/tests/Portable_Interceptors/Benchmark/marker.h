#include "ace/High_Res_Timer.h"
#include "ace/Sched_Params.h"
#include "ace/Stats.h"
#include "ace/Throughput_Stats.h"

enum class Benchmark_Method
{
  ready,
  authenticate,
  update_records
};

class Marker
{
public:
  Marker () = default;
  Marker (Marker const &) = delete;
  Marker (Marker &&) = delete;
  Marker &operator= (Marker const &) = delete;
  Marker &operator= (Marker &&) = delete;

  /// Accumulate the throughput statistics into @a throughput.
  void accumulate_into (ACE_Throughput_Stats &throughput,
                        Benchmark_Method method) const;

  /// Print the statistics.
  void dump_stats (ACE_TCHAR const *msg,
                   ACE_High_Res_Timer::global_scale_factor_type gsf,
                   Benchmark_Method method);

  /// Get a sample.
  void sample (ACE_hrtime_t throughput_diff,
               ACE_hrtime_t latency_diff,
               Benchmark_Method method);

 private:
  /// Per-thread throughput statistics for the ready method.
  ACE_Throughput_Stats throughput_method_ready_;
  /// Per-thread throughput statistics for the authenticate method.
  ACE_Throughput_Stats throughput_method_authenticate_;
  /// Per-thread throughput statistics for the update_records method.
  ACE_Throughput_Stats throughput_method_update_records_;
};
