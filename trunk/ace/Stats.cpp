// $Id$

#define ACE_BUILD_DLL
#include "ace/Stats.h"

#if !defined (__ACE_INLINE__)
# include "ace/Stats.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Stats, "$Id$")

#define ACE_STATS_INVALID_VALUE 0xFFFFFFFFu

#if !defined ACE_LACKS_LONGLONG_T
  static const ACE_UINT64 ACE_STATS_INTERNAL_OFFSET =
    ACE_UINT64_LITERAL (0x100000000);

// If ACE_LACKS_LONGLONG_T, then ACE_UINT64 is a user-defined class.
// To prevent having to construct a static of that class, declare it
// on the stack, and construct it, in each function that needs it.
#endif /* ! ACE_LACKS_LONGLONG_T */

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
          overflow_ = 1u;
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
      overflow_ = 1u;
      return -1;
    }
}

void
ACE_Stats::mean (ACE_Stats_Value &mean,
                 const ACE_UINT32 scale_factor)
{
  if (number_of_samples_ > 0)
    {
      if (cached_mean_.precision () < ACE_STATS_INVALID_VALUE)
        {
          // Use the cached mean.
          mean = cached_mean_;
        }
      else
        {
#if defined ACE_LACKS_LONGLONG_T
          const ACE_U_LongLong ACE_STATS_INTERNAL_OFFSET (0, 8);
#endif /* ACE_LACKS_LONGLONG_T */

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
                    mean);

          cached_mean_ = mean;
        }
    }
  else
    {
      mean.whole (0);
      mean.fractional (0);
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
      mean (avg, 1);
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
              // Scale up by field width so that we don't lose the
              // precision of the mean.
              const ACE_UINT64 original_sum_of_squares = sum_of_squares;

              // And do it carefully . . .
              const ACE_UINT64 product (*sample * field);
              const ACE_INT32 difference =
                ACE_U64_TO_U32 (product  -  mean_scaled);
              sum_of_squares += difference * difference;
              i.advance ();

              if (sum_of_squares < original_sum_of_squares)
                {
                  overflow_ = 1u;
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

  // Set the precision of cached_mean_ to the invalid value.
  ACE_Stats_Value invalid_stats_value (ACE_STATS_INVALID_VALUE);
  cached_mean_ = invalid_stats_value;
}

int
ACE_Stats::print_summary (const u_int precision,
                          const ACE_UINT32 scale_factor,
                          FILE *file) const
{
  if (overflow_)
    {
      ACE_OS::fprintf (file,
                       ASYS_TEXT ("ACE_Stats::print_summary: ")
                       ASYS_TEXT ("there was overflow, ")
                       ASYS_TEXT ("insufficient memory?\n"));
      return -1;
    }
  else
    {
      // Build a format string, in case the C library doesn't support %*u.
      ASYS_TCHAR format[32];
      if (precision == 0)
        ACE_OS::sprintf (format, ASYS_TEXT ("%%d"), precision);
      else
        ACE_OS::sprintf (format, ASYS_TEXT ("%%d.%%0%du"), precision);

      ACE_Stats_Value u (precision);
      ((ACE_Stats *) this)->mean (u, scale_factor);
      ASYS_TCHAR mean_string [128];
      ACE_OS::sprintf (mean_string, format, u.whole (), u.fractional ());

      ACE_Stats_Value sd (precision);
      if (((ACE_Stats *) this)->std_dev (sd, scale_factor))
        {
          ACE_OS::fprintf (file,
            ASYS_TEXT ("ACE_Stats::print_summary: there was overflow, ")
            ASYS_TEXT ("retry with smaller precision than %u?\n"),
            precision);
          return -1;
        }
      ASYS_TCHAR std_dev_string [128];
      ACE_OS::sprintf (std_dev_string, format, sd.whole (), sd.fractional ());

      ACE_Stats_Value minimum (precision), maximum (precision);
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
      ASYS_TCHAR min_string [128];
      ASYS_TCHAR max_string [128];
      ACE_OS::sprintf (min_string, format,
                       minimum.whole (), minimum.fractional ());
      ACE_OS::sprintf (max_string, format,
                       maximum.whole (), maximum.fractional ());

      ACE_OS::fprintf (file, ASYS_TEXT ("samples: %u (%s - %s); mean: ")
                       ASYS_TEXT ("%s; std dev: %s\n"),
                       samples (), min_string, max_string,
                       mean_string, std_dev_string);

      return 0;
    }
}

void
ACE_Stats::dump (void) const
{
  print_summary (3u);
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

      // Fractional = (dividend % divisor) * 10^precision / divisor.
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Node <ACE_INT32>;
template class ACE_Unbounded_Queue <ACE_INT32>;
template class ACE_Unbounded_Queue_Iterator <ACE_INT32>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Node <ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue <ACE_INT32>
#pragma instantiate ACE_Unbounded_Queue_Iterator <ACE_INT32>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
