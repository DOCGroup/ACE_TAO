//
// $Id$
//

ACE_INLINE
ECT_Driver::Latency_Stats::Latency_Stats (void)
  :  n_ (0),
     sum_ (0),
     sum2_ (0),
     min_ (INT_MAX),
     max_ (INT_MIN)
{
}

ACE_INLINE
void ECT_Driver::Latency_Stats::sample (ACE_hrtime_t sample)
{
  this->n_ ++;
  this->sum_ += sample;
  this->sum2_ += sample * sample;
  if (this->min_ > sample)
    this->min_ = sample;
  if (this->max_ < sample)
    this->max_ = sample;
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
