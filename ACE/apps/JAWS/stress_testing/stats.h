#include "global.h"
#include <vector>

#ifndef _D_Stats
#define _D_Stats

#define THREAD_START 42
#define THREAD_END 43

class Init_Fini_t {
public:
  int type {}; // 0 is start, 1 is end
  ACE_Time_Value timestamp;
};

class Stats {
public:
  explicit Stats (int);
  Stats (Stats const &) = delete;
  Stats (Stats &&) = delete;
  Stats &operator= (Stats const &) = delete;
  Stats &operator= (Stats &&) = delete;
  void log (int, float, float);
  void i_have_started (int);
  void i_am_done (int);
  void output ();
private:
  std::vector<float> throughput_;
  std::vector<float> latency_;
  std::vector<Init_Fini_t> init_fini_; // Two entries per thread for peak activity.
  int thread_count_ {};
};
#endif
