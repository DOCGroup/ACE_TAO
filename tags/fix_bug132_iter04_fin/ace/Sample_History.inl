// $Id$

ACE_INLINE int
ACE_Sample_History::sample (ACE_UINT64 value)
{
  if (this->sample_count_ >= this->max_samples_)
    return -1;

  this->samples_[this->sample_count_++] = value;
  return 0;
}
