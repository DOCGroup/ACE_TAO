// $Id$

ACE_INLINE int
EC_Driver::verbose (void) const
{
  return this->verbose_;
}

// ****************************************************************

ACE_INLINE
EC_Driver::Latency_Stats::Latency_Stats (void)
  :  n_ (0),
     sum_ (0),
     sum2_ (0),
     min_ (0),
     max_ (0)
{
}

ACE_INLINE
void EC_Driver::Latency_Stats::sample (ACE_hrtime_t sample)
{
  this->sum_ += sample;
  this->sum2_ += sample * sample;
  if (this->n_ == 0)
    {
      this->min_ = sample;
      this->max_ = sample;
    }
  else if (this->min_ > sample)
    this->min_ = sample;
  else if (this->max_ < sample)
    this->max_ = sample;
  this->n_++;
}

// ****************************************************************

ACE_INLINE
EC_Driver::Throughput_Stats::Throughput_Stats (void)
  :  n_ (0),
     done_ (0)
{
}

ACE_INLINE void
EC_Driver::Throughput_Stats::start (void)
{
  this->done_ = 0;
  this->start_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
EC_Driver::Throughput_Stats::stop (void)
{
  this->stop_ = ACE_OS::gethrtime ();
  this->done_ = 1;
}

ACE_INLINE void
EC_Driver::Throughput_Stats::sample (void)
{
  this->n_++;
}
