#include "marker.h"

void
Marker::accumulate_into (ACE_Throughput_Stats &throughput,
                         Benchmark_Method method) const
{
  switch (method)
    {
    case Benchmark_Method::ready:
      throughput.accumulate (this->throughput_method_ready_);
      break;
    case Benchmark_Method::authenticate:
      throughput.accumulate (this->throughput_method_authenticate_);
      break;
    case Benchmark_Method::update_records:
      throughput.accumulate (this->throughput_method_update_records_);
      break;
    default:
      break;
    }

}

void
Marker::dump_stats (ACE_TCHAR const *msg,
                    ACE_High_Res_Timer::global_scale_factor_type gsf,
                    Benchmark_Method method)
{
  switch (method)
    {
    case Benchmark_Method::ready:
      this->throughput_method_ready_.dump_results (msg, gsf);
      break;
    case Benchmark_Method::authenticate:
      this->throughput_method_authenticate_.dump_results (msg, gsf);
      break;
    case Benchmark_Method::update_records:
      this->throughput_method_update_records_.dump_results (msg, gsf);
      break;
    default:
      break;
    }
}

void
Marker::sample (ACE_hrtime_t throughput_diff,
                ACE_hrtime_t latency_diff,
                Benchmark_Method method)
{
  switch (method)
    {
    case Benchmark_Method::ready:
      this->throughput_method_ready_.sample (throughput_diff,
                                             latency_diff);
      break;
    case Benchmark_Method::authenticate:
      this->throughput_method_authenticate_.sample (throughput_diff,
                                                    latency_diff);
      break;
    case Benchmark_Method::update_records:
      this->throughput_method_update_records_.sample (throughput_diff,
                                                      latency_diff);
      break;
    default:
      break;
    }

}
