// $Id$

#include "ace/Timer/Basic_Stats.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#if !defined (__ACE_INLINE__)
#include "ace/Timer/Basic_Stats.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Basic_Stats, "$Id$")

void
ACE_Basic_Stats::accumulate (const ACE_Basic_Stats &rhs)
{
  if (rhs.samples_count_ == 0)
    return;

  if (this->samples_count_ == 0)
    {
      this->samples_count_ = rhs.samples_count_;

      this->min_  = rhs.min_;
      this->max_  = rhs.max_;
      this->sum_  = rhs.sum_;
      this->sum2_ = rhs.sum2_;

      return;
    }
  this->samples_count_ += rhs.samples_count_;

  if (this->min_ > rhs.min_)
    this->min_ = rhs.min_;
  if (this->max_ < rhs.max_)
    this->max_ = rhs.max_;

  this->sum_  += rhs.sum_;
  this->sum2_ += rhs.sum2_;
}

#ifdef ACE_SUBSET_0
void
ACE_Basic_Stats::dump_results (const ACE_TCHAR *msg,
                               ACE_UINT32 sf) const
{
  if (this->samples_count () == 0u)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("%s : no data collected\n"), msg));
      return;
    }

  ACE_UINT64 avg = this->sum_ / this->samples_count_;
  ACE_UINT64 dev =
#if defined ACE_LACKS_LONGLONG_T
    ACE_static_cast (ACE_U_LongLong,
                     this->sum2_ / this->samples_count_)
    - avg * ACE_U64_TO_U32(avg);
#else  /* ! ACE_LACKS_LONGLONG_T */
    this->sum2_ / this->samples_count_ - avg * avg;
#endif /* ! ACE_LACKS_LONGLONG_T */

  double l_min = ACE_CU64_TO_CU32 (this->min_) / sf;
  double l_max = ACE_CU64_TO_CU32 (this->max_) / sf;
  double l_avg = ACE_CU64_TO_CU32 (avg) / sf;
  double l_dev = ACE_CU64_TO_CU32 (dev) / (sf * sf);

  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("%s latency   : %.2f[%d]/%.2f/%.2f[%d]/%.2f (min/avg/max/var^2)\n"),
              msg,
              l_min, this->min_at_,
              l_avg,
              l_max, this->max_at_,
              l_dev));
}
#endif /* ACE_SUBSET_0 */
