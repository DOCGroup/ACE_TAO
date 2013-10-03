// $Id$

class Latency_Stats
{
public:
  Latency_Stats (void);

  void dump_results (const ACE_TCHAR* test_name,
                     const ACE_TCHAR* sub_test);

  void sample (ACE_hrtime_t sample);

  void accumulate (const Latency_Stats& stats);
  // Useful to merge several Latency_Stats.

private:
  u_long n_;
  ACE_hrtime_t sum_;
  ACE_hrtime_t sum2_;
  ACE_hrtime_t min_;
  ACE_hrtime_t max_;
};

inline
Latency_Stats::Latency_Stats (void)
  :  n_ (0),
     sum_ (0),
     sum2_ (0),
     min_ (0),
     max_ (0)
{
}

inline void
Latency_Stats::sample (ACE_hrtime_t sample)
{
  this->sum_  += sample;
  this->sum2_ += sample * sample;
  if (this->n_ == 0)
    {
      this->min_ = sample;
      this->max_ = sample;
    }
  if (this->min_ > sample)
    this->min_ = sample;
  if (this->max_ < sample)
    this->max_ = sample;
  this->n_++;
}

inline void
Latency_Stats::dump_results (const ACE_TCHAR *test_name,
                             const ACE_TCHAR *sub_test)
{
  if (this->n_ < 1)
    return;

  ACE_hrtime_t avg = this->sum_ / this->n_;
  ACE_hrtime_t dev =
    this->sum2_ / this->n_ - avg*avg;
  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();

  ACE_UINT64 min_usec = this->min_ / gsf;
  ACE_UINT64 max_usec = this->max_ / gsf;
  ACE_UINT64 avg_usec = avg / gsf;
  ACE_UINT64 dev_usec = dev / (gsf * gsf);
  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: %.2f/%.2f/%.2f/%.2f (min/avg/max/var^2) [usecs]\n",
              test_name, sub_test,
              min_usec, avg_usec, max_usec, dev_usec));
}

inline void
Latency_Stats::accumulate (const Latency_Stats& rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
      this->n_    = rhs.n_;
      this->min_  = rhs.min_;
      this->max_  = rhs.max_;
      this->sum_  = rhs.sum_;
      this->sum2_ = rhs.sum2_;
      return;
    }

  if (this->min_ > rhs.min_)
    this->min_ = rhs.min_;
  if (this->max_ < rhs.max_)
    this->max_ = rhs.max_;

  this->sum_  += rhs.sum_;
  this->sum2_ += rhs.sum2_;
  this->n_    += rhs.n_;
}

class Throughput_Stats
{
public:
  Throughput_Stats (void);

  void dump_results (const ACE_TCHAR* test_name,
                     const ACE_TCHAR* sub_test);

  void sample (void);
  // An event has been received

  void accumulate (const Throughput_Stats& stats);
  // Useful to merge several Throughput_Stats.

private:
  u_long n_;
  ACE_hrtime_t start_;
  ACE_hrtime_t stop_;
};

inline void
Throughput_Stats::accumulate (const Throughput_Stats& rhs)
{
  if (rhs.n_ == 0)
    return;

  if (this->n_ == 0)
    {
      this->start_ = rhs.start_;
      this->stop_ = rhs.stop_;
      this->n_ = rhs.n_;
      return;
    }

  if (this->start_ > rhs.start_)
    this->start_ = rhs.start_;

  if (this->stop_ < rhs.stop_)
    this->stop_ = rhs.stop_;

  this->n_ += rhs.n_;
}

inline void
Throughput_Stats::dump_results (const ACE_TCHAR *test_name,
                                const ACE_TCHAR *subtest)
{
  if (this->n_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s/%s: no events recorded\n",
                  test_name, subtest));
      return;
    }

  ACE_Time_Value tv;
  ACE_High_Res_Timer::hrtime_to_tv (tv, this->stop_ - this->start_);

  double f = 1.0/(tv.sec () + tv.usec () / 1000000.0);
  double events_per_second = this->n_ * f;

  ACE_DEBUG ((LM_DEBUG,
              "%s/%s: "
              "%d / %d.%06.6d = %.3f events/second\n",
              test_name, subtest,
              this->n_,
              tv.sec (), tv.usec (),
              events_per_second));
}

inline
Throughput_Stats::Throughput_Stats (void)
  : n_ (0),
    start_ (),
    stop_ ()
{
}

inline void
Throughput_Stats::sample (void)
{
  if (this->n_ == 0)
    {
      this->start_ = ACE_OS::gethrtime ();
    }
  this->n_++;
  this->stop_ = ACE_OS::gethrtime ();
}

inline void
move_to_rt_class (void)
{
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;

  int result = ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                                       priority,
                                                       ACE_SCOPE_PROCESS));
  if (result == 0)
    {
      result = ACE_OS::thr_setprio (priority);
    }

  if (result != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Cannot move program to realtime class.\n"));
    }
}
