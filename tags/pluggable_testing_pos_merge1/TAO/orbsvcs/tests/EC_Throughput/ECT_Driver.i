//
// $Id$
//

ACE_INLINE
ECT_Driver::Latency_Stats::Latency_Stats (void)
  :  n_ (0),
     sum_ (0),
     sum2_ (0),
     min_ (0),
     max_ (0)
{
}

ACE_INLINE
void ECT_Driver::Latency_Stats::sample (ACE_hrtime_t sample)
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

ACE_INLINE void
ECT_Driver::Throughput_Stats::start (void)
{
  this->done_ = 0;
  this->start_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
ECT_Driver::Throughput_Stats::stop (void)
{
  this->done_ = 1;
  this->stop_ = ACE_OS::gethrtime ();
}

ACE_INLINE void
ECT_Driver::Throughput_Stats::sample (void)
{
  this->n_++;
}

// ****************************************************************

ACE_INLINE
void ECT_Driver::end_to_end (ACE_hrtime_t sample)
{
  this->end_to_end_.sample (sample);
}

ACE_INLINE
void ECT_Driver::supplier_to_ec (ACE_hrtime_t sample)
{
  this->supplier_to_ec_.sample (sample);
}

ACE_INLINE
void ECT_Driver::inside_ec (ACE_hrtime_t sample)
{
  this->inside_ec_.sample (sample);
}

ACE_INLINE
void ECT_Driver::ec_to_consumer (ACE_hrtime_t sample)
{
  this->ec_to_consumer_.sample (sample);
}

