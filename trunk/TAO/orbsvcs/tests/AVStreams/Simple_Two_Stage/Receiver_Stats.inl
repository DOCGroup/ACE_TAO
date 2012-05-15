// $Id$

ACE_INLINE
Receiver_Stats::Receiver_Stats (void)
  : samples_count_ (0)
  , min_ (0)
  , min_at_ (0)
  , max_ (0)
  , max_at_ (0)
  , sum_ (0)
  , sum2_ (0)
{
}

ACE_INLINE ACE_UINT32
Receiver_Stats::samples_count (void) const
{
  return this->samples_count_;
}

ACE_INLINE void
Receiver_Stats::sample (ACE_UINT64 value)
{
  ++this->samples_count_;

  if (this->samples_count_ == 1u)
    {
      this->min_ = value;
      this->min_at_ = this->samples_count_;
      this->max_ = value;
      this->max_at_ = this->samples_count_;
      this->sum_ = value;
      this->sum2_ = value * value;
    }
  else
    {
      if (this->min_ > value)
        {
          this->min_ = value;
          this->min_at_ = this->samples_count_;
        }
      if (this->max_ < value)
        {
          this->max_ = value;
          this->max_at_ = this->samples_count_;
        }

      this->sum_  += value;
      this->sum2_ += value * value;
    }
}
