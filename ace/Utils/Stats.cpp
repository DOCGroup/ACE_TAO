// $Id$

#include "ace/Utils/Stats.h"
#include "ace/Timer/High_Res_Timer.h"

#if !defined (__ACE_INLINE__)
# include "ace/Utils/Stats.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Stats, "$Id$")

ACE_UINT32
ACE_Stats_Value::fractional_field (void) const
{
  if (precision () == 0)
    {
      return 1;
    }
  else
    {
      ACE_UINT32 field = 10;
      for (u_int i = 0; i < precision () - 1; ++i)
        {
          field *= 10;
        }

      return field;
    }
}

int
ACE_Stats::sample (const ACE_INT32 value)
{
  if (samples_.enqueue_tail (value) == 0)
    {
      ++number_of_samples_;
      if (number_of_samples_ == 0)
        {
          // That's a lot of samples :-)
          overflow_ = EFAULT;
          return -1;
        }

      if (value < min_)
        min_ = value;

      if (value > max_)
        max_ = value;

      return 0;
    }
  else
    {
      // Probably failed due to running out of memory when trying to
      // enqueue the new value.
      overflow_ = errno;
      return -1;
    }
}

void
ACE_Stats::mean (ACE_Stats_Value &m,
                 const ACE_UINT32 scale_factor)
{
  if (number_of_samples_ > 0)
    {
#if defined ACE_LACKS_LONGLONG_T
      // If ACE_LACKS_LONGLONG_T, then ACE_UINT64 is a user-defined class.
      // To prevent having to construct a static of that class, declare it
      // on the stack, and construct it, in each function that needs it.
      const ACE_U_LongLong ACE_STATS_INTERNAL_OFFSET (0, 8);
#else  /* ! ACE_LACKS_LONGLONG_T */
      const ACE_UINT64 ACE_STATS_INTERNAL_OFFSET =
        ACE_UINT64_LITERAL (0x100000000);
#endif /* ! ACE_LACKS_LONGLONG_T */

      ACE_UINT64 sum = ACE_STATS_INTERNAL_OFFSET;
      ACE_Unbounded_Queue_Iterator<ACE_INT32> i (samples_);
      while (! i.done ())
        {
          ACE_INT32 *sample;
          if (i.next (sample))
            {
              sum += *sample;
              i.advance ();
            }
        }

      // sum_ was initialized with ACE_STATS_INTERNAL_OFFSET, so
      // subtract that off here.
      quotient (sum - ACE_STATS_INTERNAL_OFFSET,
                number_of_samples_ * scale_factor,
                m);
    }
  else
    {
      m.whole (0);
      m.fractional (0);
    }
}

int
ACE_Stats::std_dev (ACE_Stats_Value &std_dev,
                    const ACE_UINT32 scale_factor)
{
  if (number_of_samples_ <= 1)
    {
      std_dev.whole (0);
      std_dev.fractional (0);
    }
  else
    {
      const ACE_UINT32 field = std_dev.fractional_field ();

      // The sample standard deviation is:
      //
      // sqrt (sum (sample_i - mean)^2 / (number_of_samples_ - 1))

      ACE_UINT64 mean_scaled;
      // Calculate the mean, scaled, so that we don't lose its
      // precision.
      ACE_Stats_Value avg (std_dev.precision ());
      mean (avg, 1u);
      avg.scaled_value (mean_scaled);

      // Calculate the summation term, of squared differences from the
      // mean.
      ACE_UINT64 sum_of_squares = 0;
      ACE_Unbounded_Queue_Iterator<ACE_INT32> i (samples_);
      while (! i.done ())
        {
          ACE_INT32 *sample;
          if (i.next (sample))
            {
              const ACE_UINT64 original_sum_of_squares = sum_of_squares;

              // Scale up by field width so that we don't lose the
              // precision of the mean.  Carefully . . .
              const ACE_UINT64 product (*sample * field);

              ACE_UINT64 difference;
              // NOTE: please do not reformat this code!  It //
              // works with the Diab compiler the way it is! //
              if  (product >= mean_scaled)                   //
                {                                            //
                  difference = product - mean_scaled;        //
                }                                            //
              else                                           //
                {                                            //
                  difference = mean_scaled - product;        //
                }                                            //
              // NOTE: please do not reformat this code!  It //
              // works with the Diab compiler the way it is! //

              // Square using 64-bit arithmetic.
              sum_of_squares += difference * ACE_U64_TO_U32 (difference);
              i.advance ();

              if (sum_of_squares < original_sum_of_squares)
                {
                  overflow_ = ENOSPC;
                  return -1;
                }
            }
        }

      // Divide the summation by (number_of_samples_ - 1), to get the
      // variance.  In addition, scale the variance down to undo the
      // mean scaling above.  Otherwise, it can get too big.
      ACE_Stats_Value variance (std_dev.precision ());
      quotient (sum_of_squares,
                (number_of_samples_ - 1) * field * field,
                variance);

      // Take the square root of the variance to get the standard
      // deviation.  First, scale up . . .
      ACE_UINT64 scaled_variance;
      variance.scaled_value (scaled_variance);

      // And scale up, once more, because we'll be taking the square
      // root.
      scaled_variance *= field;
      ACE_Stats_Value unscaled_standard_deviation (std_dev.precision ());
      square_root (scaled_variance,
                   unscaled_standard_deviation);

      // Unscale.
      quotient (unscaled_standard_deviation,
                scale_factor * field,
                std_dev);
    }

  return 0;
}


void
ACE_Stats::reset (void)
{
  overflow_ = 0u;
  number_of_samples_ = 0u;
  min_ = 0x7FFFFFFF;
  max_ = -0x8000 * 0x10000;
  samples_.reset ();
}

int
ACE_Stats::print_summary (const u_int precision,
                          const ACE_UINT32 scale_factor,
                          FILE *file) const
{
  ACE_TCHAR mean_string [128];
  ACE_TCHAR std_dev_string [128];
  ACE_TCHAR min_string [128];
  ACE_TCHAR max_string [128];
  int success = 0;

  for (int tmp_precision = precision;
       ! overflow_  &&  ! success  &&  tmp_precision >= 0;
       --tmp_precision)
    {
      // Build a format string, in case the C library doesn't support %*u.
      ACE_TCHAR format[32];
      if (tmp_precision == 0)
        ACE_OS::sprintf (format, ACE_LIB_TEXT ("%%%d"), tmp_precision);
      else
        ACE_OS::sprintf (format, ACE_LIB_TEXT ("%%d.%%0%du"), tmp_precision);

      ACE_Stats_Value u (tmp_precision);
      ((ACE_Stats *) this)->mean (u, scale_factor);
      ACE_OS::sprintf (mean_string, format, u.whole (), u.fractional ());

      ACE_Stats_Value sd (tmp_precision);
      if (((ACE_Stats *) this)->std_dev (sd, scale_factor))
        {
          success = 0;
          continue;
        }
      else
        {
          success = 1;
        }
      ACE_OS::sprintf (std_dev_string, format, sd.whole (), sd.fractional ());

      ACE_Stats_Value minimum (tmp_precision), maximum (tmp_precision);
      if (min_ != 0)
        {
          const ACE_UINT64 m (min_);
          quotient (m, scale_factor, minimum);
        }
      if (max_ != 0)
        {
          const ACE_UINT64 m (max_);
          quotient (m, scale_factor, maximum);
        }
      ACE_OS::sprintf (min_string, format,
                       minimum.whole (), minimum.fractional ());
      ACE_OS::sprintf (max_string, format,
                       maximum.whole (), maximum.fractional ());
    }

  if (success == 1)
    {
      ACE_OS::fprintf (file, ACE_LIB_TEXT ("samples: %u (%s - %s); mean: ")
                       ACE_LIB_TEXT ("%s; std dev: %s\n"),
                       samples (), min_string, max_string,
                       mean_string, std_dev_string);
      return 0;
    }
  else
    {
#if !defined (ACE_HAS_WINCE)
      ACE_OS::fprintf (file,
                       ACE_LIB_TEXT ("ACE_Stats::print_summary: OVERFLOW: %s\n"),
                       strerror (overflow_));
#else
      // WinCE doesn't have strerror ;(
      ACE_OS::fprintf (file,
                       ACE_LIB_TEXT ("ACE_Stats::print_summary: OVERFLOW\n"));
#endif /* ACE_HAS_WINCE */
      return -1;
    }
}

void
ACE_Stats::quotient (const ACE_UINT64 dividend,
                     const ACE_UINT32 divisor,
                     ACE_Stats_Value &quotient)
{
  // The whole part of the division comes from simple integer division.
  quotient.whole (ACE_static_cast (ACE_UINT32,
    divisor == 0  ?  0  :  dividend / divisor));

  if (quotient.precision () > 0  ||  divisor == 0)
    {
      const ACE_UINT32 field = quotient.fractional_field ();

      // Fractional = (dividend % divisor) * 10^precision / divisor

      // It would be nice to add round-up term:
      // Fractional = (dividend % divisor) * 10^precision / divisor  +
      //                10^precision/2 / 10^precision
      //            = ((dividend % divisor) * 10^precision  +  divisor) /
      //                divisor
      quotient.fractional (ACE_static_cast (ACE_UINT32,
        dividend % divisor * field / divisor));
    }
  else
    {
      // No fractional portion is requested, so don't bother
      // calculating it.
      quotient.fractional (0);
    }
}

void
ACE_Stats::quotient (const ACE_Stats_Value &dividend,
                     const ACE_UINT32 divisor,
                     ACE_Stats_Value &quotient)
{
  // The whole part of the division comes from simple integer division.
  quotient.whole (divisor == 0  ?  0  :  dividend.whole () / divisor);

  if (quotient.precision () > 0  ||  divisor == 0)
    {
      const ACE_UINT32 field = quotient.fractional_field ();

      // Fractional = (dividend % divisor) * 10^precision / divisor.
      quotient.fractional (dividend.whole () % divisor * field / divisor  +
                           dividend.fractional () / divisor);
    }
  else
    {
      // No fractional portion is requested, so don't bother
      // calculating it.
      quotient.fractional (0);
    }
}

void
ACE_Stats::square_root (const ACE_UINT64 n,
                        ACE_Stats_Value &square_root)
{
  ACE_UINT32 floor = 0;
  ACE_UINT32 ceiling = 0xFFFFFFFFu;
  ACE_UINT32 mid = 0;
  u_int i;

  // The maximum number of iterations is log_2 (2^64) == 64.
  for (i = 0; i < 64; ++i)
    {
      mid = (ceiling - floor) / 2  +  floor;
      if (floor == mid)
        // Can't divide the interval any further.
        break;
      else
        {
          // Multiply carefully to avoid overflow.
          ACE_UINT64 mid_squared = mid; mid_squared *= mid;
          if (mid_squared == n)
            break;
          else if (mid_squared < n)
            floor = mid;
          else
            ceiling = mid;
        }
    }

  square_root.whole (mid);
  ACE_UINT64 mid_squared = mid; mid_squared *= mid;

  if (square_root.precision ()  &&  mid_squared < n)
    {
      // (mid * 10^precision + fractional)^2 ==
      //   n^2 * 10^(precision * 2)

      const ACE_UINT32 field = square_root.fractional_field ();

      floor = 0;
      ceiling = field;
      mid = 0;

      // Do the 64-bit arithmetic carefully to avoid overflow.
      ACE_UINT64 target = n;
      target *= field;
      target *= field;

      ACE_UINT64 difference = 0;

      for (i = 0; i < square_root.precision (); ++i)
        {
          mid = (ceiling - floor) / 2 + floor;

          ACE_UINT64 current = square_root.whole () * field  +  mid;
          current *= square_root.whole () * field  +  mid;

          if (floor == mid)
            {
              difference = target - current;
              break;
            }
          else if (current <= target)
            floor = mid;
          else
            ceiling = mid;
        }

      // Check to see if the fractional part should be one greater.
      ACE_UINT64 next = square_root.whole () * field  +  mid + 1;
      next *= square_root.whole () * field  +  mid + 1;

      square_root.fractional (next - target < difference  ?  mid + 1  :  mid);
    }
  else
    {
      // No fractional portion is requested, so don't bother
      // calculating it.
      square_root.fractional (0);
    }
}

// ****************************************************************

ACE_Throughput_Stats::ACE_Throughput_Stats (void)
  : ACE_Basic_Stats ()
  , throughput_last_ (0)
#if 0
  // @@TODO: This is what I really wanted to compute, but it just
  // does not work.
  , throughput_sum_x_ (0)
  , throughput_sum_x2_ (0)
  , throughput_sum_y_ (0)
  , throughput_sum_y2_ (0)
  , throughput_sum_xy_ (0)
#endif /* 0 */
{
}

void
ACE_Throughput_Stats::sample (ACE_UINT64 throughput,
                              ACE_UINT64 latency)
{
  this->ACE_Basic_Stats::sample (latency);

  if (this->samples_count () == 1u)
    {

      this->throughput_last_   = throughput;
#if 0
      // @@TODO: This is what I really wanted to compute, but it just
      // does not work.
      this->throughput_sum_y_  = this->samples_count_;
      this->throughput_sum_y2_ = this->samples_count_ * this->samples_count_;
      this->throughput_sum_x_  = throughput;
      this->throughput_sum_x2_ = throughput * throughput;
      this->throughput_sum_xy_ = throughput * this->samples_count_;

      printf ("%f %qu\n", throughput / 400000000.0, this->samples_count_);
#endif /* 0 */
    }
  else
    {
      this->throughput_last_ = throughput;

#if 0
      // @@TODO: This is what I really wanted to compute, but it just
      // does not work.
      this->throughput_sum_y_  += this->samples_count_;
      this->throughput_sum_y2_ += this->samples_count_ * this->samples_count_;
      this->throughput_sum_x_  += throughput;
      this->throughput_sum_x2_ += throughput * throughput;
      this->throughput_sum_xy_ += throughput * this->samples_count_;

      printf ("%f %qu\n", throughput / 400000000.0, this->samples_count_);
#endif /* 0 */
    }
}

void
ACE_Throughput_Stats::accumulate (const ACE_Throughput_Stats &rhs)
{
  if (rhs.samples_count () == 0u)
    return;

  this->ACE_Basic_Stats::accumulate (rhs);

  if (this->samples_count () == 0u)
    {
      this->throughput_last_   = rhs.throughput_last_;
#if 0
      // @@TODO: This is what I really wanted to compute, but it just
      // does not work.
      this->throughput_sum_x_  = rhs.throughput_sum_x_;
      this->throughput_sum_x2_ = rhs.throughput_sum_x2_;
      this->throughput_sum_y_  = rhs.throughput_sum_y_;
      this->throughput_sum_y2_ = rhs.throughput_sum_y2_;
      this->throughput_sum_xy_ = rhs.throughput_sum_xy_;
#endif /* 0 */

      return;
    }


  if (this->throughput_last_ < rhs.throughput_last_)
    this->throughput_last_ = rhs.throughput_last_;

#if 0
  // @@TODO: This is what I really wanted to compute, but it just
  // does not work.
  this->throughput_sum_x_  += rhs.throughput_sum_x_;
  this->throughput_sum_x2_ += rhs.throughput_sum_x2_;
  this->throughput_sum_y_  += rhs.throughput_sum_y_;
  this->throughput_sum_y2_ += rhs.throughput_sum_y2_;
  this->throughput_sum_xy_ += rhs.throughput_sum_xy_;
#endif /* 0 */
}

void
ACE_Throughput_Stats::dump_results (const ACE_TCHAR* msg,
                                    ACE_UINT32 sf)
{
  if (this->samples_count () == 0u)
    {
#ifdef ACE_SUBSET_0
      ACE_DEBUG ((LM_DEBUG,
                  ACE_LIB_TEXT ("%s : no data collected\n"), msg));
#endif
      return;
    }

  this->ACE_Basic_Stats::dump_results (msg, sf);

  ACE_Throughput_Stats::dump_throughput (msg, sf,
                                         this->throughput_last_,
                                         this->samples_count ());

#if 0
  // @@TODO: This is what I really wanted to generate, but it just
  // doesn't work.
  double t_sum_x =
    ACE_CU64_TO_CU32 (this->throughput_sum_x_);// / sf);
  //t_sum_x /= 1000000.0;
  double t_sum_y =
    ACE_CU64_TO_CU32 (this->throughput_sum_y_);
  double t_sum_x2 =
    ACE_CU64_TO_CU32 (this->throughput_sum_x2_);// / (sf*sf));
  //t_sum_x2 /= 1000000.0;
  //t_sum_x2 /= 1000000.0;
  double t_sum_y2 =
    ACE_CU64_TO_CU32 (this->throughput_sum_y2_);
  double t_sum_xy =
    ACE_CU64_TO_CU32 (this->throughput_sum_xy_);// / sf);
  //t_sum_xy /= 1000000.0;
  double t_avgx = t_sum_x / this->samples_count ();
  double t_avgy = t_sum_y / this->samples_count ();

  double t_a =
    (this->samples_count () * t_sum_xy - t_sum_x * t_sum_y)
    / (this->samples_count () * t_sum_x2 - t_sum_x * t_sum_x);
  double t_b = (t_avgy - t_a * t_avgx);

  t_a *= 1000000.0;

  double d_r =
    (t_sum_xy - t_avgx * t_sum_y - t_avgy * t_sum_x
     + this->samples_count () * t_avgx * t_avgy);
  double n_r =
    (t_sum_x2
     - this->samples_count () * t_avgx * t_avgx)
    * (t_sum_y2
       - this->samples_count () * t_avgy * t_avgy);
  double t_r = d_r * d_r / n_r;

  //  ACE_DEBUG ((LM_DEBUG,
  //              "%s throughput: %.2f/%.2f/%.2f/%.6f/%.2f (avg/a/b/r/elapsed)\n",
  //              msg, t_avg, t_a, t_b, t_r, seconds));
  //  ACE_DEBUG ((LM_DEBUG,
  //              "%s        data: %.2f/%.2f/%.2f/%.6f/%.2f (x/x2/y/y2/xy)\n",
  //              msg, t_sum_x, t_sum_x2, t_sum_y, t_sum_y2, t_sum_xy));
#endif
}

void
ACE_Throughput_Stats::dump_throughput (const ACE_TCHAR *msg,
                                       ACE_UINT32 sf,
                                       ACE_UINT64 elapsed_time,
                                       ACE_UINT32 samples_count)
{
  double seconds =
#if defined ACE_LACKS_LONGLONG_T
    elapsed_time / sf;
#else  /* ! ACE_LACKS_LONGLONG_T */
    ACE_static_cast (double,
                     ACE_UINT64_DBLCAST_ADAPTER(elapsed_time / sf));
#endif /* ! ACE_LACKS_LONGLONG_T */
  seconds /= ACE_HR_SCALE_CONVERSION; 
  double t_avg = samples_count / seconds;

#ifdef ACE_SUBSET_0
  ACE_DEBUG ((LM_DEBUG,
              ACE_LIB_TEXT ("%s throughput: %.2f (events/second)\n"),
              msg, t_avg));
#endif
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <ACE_INT32>;
template class ACE_Unbounded_Queue <ACE_INT32>;
template class ACE_Unbounded_Queue_Iterator <ACE_INT32>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node <ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue <ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue_Iterator <ACE_INT32>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
