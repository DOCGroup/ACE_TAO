// $Id$

#define ACE_BUILD_DLL
#include "ace/Stats.h"

#if !defined (__ACE_INLINE__)
# include "ace/Stats.i"
#endif /* __ACE_INLINE__ */

#if !defined ACE_LACKS_LONGLONG_T
// 1) If ACE_LACKS_LONGLONG_T, then ACE_UINT64 is a user-defined class.
//    To prevent having to construct a static of that class, declare it
//    on the stack, and construct it in, each function that needs it.
// 2) To avoid warnings from some compilers, split the 0x100000000ull
//    constant into two pieces.

// @@ David, do we need an ACE_UINT64_LITERAL(X) macro here?

# if defined (ACE_WIN32)
static const ACE_UINT64 ACE_STATS_INTERNAL_OFFSET = 0x100000000ui64;
# else
static const ACE_UINT64 ACE_STATS_INTERNAL_OFFSET = 0x10000ull * 0x10000ull;
# endif
#endif /* ! ACE_LACKS_LONGLONG_T */

ACE_UINT32
ACE_Stats_Value::fractional_field (void) const
{
  ACE_UINT32 field = 10;
  for (u_int i = 0; i < precision () - 1; ++i)
    field *= 10;

  return field;
}

int
ACE_Stats::sample (const ACE_INT32 value)
{
  if (overflow_)
    {
      // Had already overflowed.
      return -1;
    }
  else
    {
      ++samples_;
      if (samples_ == 0)
        {
          overflow_ = 1;
          --samples_;
          return -1;
        }
      else
        {
          ACE_UINT64 old_sum = sum_;
          ACE_UINT64 old_sum_of_squares = sum_of_squares_;

          sum_ += value;
          sum_of_squares_ += value * value;

          // Overflow checks.
          if ((value >= 0  ?  sum_ < old_sum  :  sum_ > old_sum)  ||
              sum_of_squares_ < old_sum_of_squares)
            {
              overflow_ = 1;
              --samples_;
              sum_ = old_sum;
              sum_of_squares_ = old_sum_of_squares;
              return -1;
            }

          if (value < min_)
            min_ = value;

          if (value > max_)
            max_ = value;

          return 0;
        }
    }
}

void
ACE_Stats::mean (ACE_Stats_Value &mean,
                 const ACE_UINT32 scale_factor) const
{
  if (samples_ > 0)
  {
#if defined ACE_LACKS_LONGLONG_T
    const ACE_U_LongLong ACE_STATS_INTERNAL_OFFSET (0, 8);
#endif /* ACE_LACKS_LONGLONG_T */

    quotient (sum_ - ACE_STATS_INTERNAL_OFFSET,
              samples_ * scale_factor,
              mean);
  }
}

void
ACE_Stats::std_dev (ACE_Stats_Value &std_dev,
                    const ACE_UINT32 scale_factor) const
{
  if (samples_ <= 1)
    {
      std_dev.whole (0);
      std_dev.fractional (0);
    }
  else
    {
#if defined ACE_LACKS_LONGLONG_T
      const ACE_U_LongLong ACE_STATS_INTERNAL_OFFSET (0, 8);
#endif /* ACE_LACKS_LONGLONG_T */
      // The sample standard deviation is:
      //
      // sqrt ((sum_of_squares_ - sums^2/samples_) / (samples_-1))

      // Calculate the square of sums divided by the number of samples,
      // carefully putting it into a 64-bit integer.
      const ACE_UINT64 sums_squared_over_samples =
        (sum_ - ACE_STATS_INTERNAL_OFFSET) *
        ((sum_ - ACE_STATS_INTERNAL_OFFSET) / samples_);

      // Divide the difference by (samples_ - 1), to get the variance.
      // Don't scale the result, yet.
      ACE_Stats_Value variance (std_dev.precision ());

      quotient (sum_of_squares_ - sums_squared_over_samples,
                samples_ - 1,
                variance);

      const ACE_UINT32 field = std_dev.fractional_field ();

      // Take the square root of the variance to get the standard
      // deviation.
      ACE_UINT64 scaled_variance;
      variance.scaled_value (scaled_variance);
      // Scale up, once more, because we'll be taking the square root.
      scaled_variance *= field;
      ACE_Stats_Value unscaled_standard_deviation (std_dev.precision ());
      square_root (scaled_variance,
                   unscaled_standard_deviation);

      // Unscale.
      quotient (unscaled_standard_deviation,
                scale_factor * field,
                std_dev);
    }
}


void
ACE_Stats::reset (void)
{
#if defined ACE_LACKS_LONGLONG_T
  const ACE_U_LongLong ACE_STATS_INTERNAL_OFFSET (0, 8);
#endif /* ACE_LACKS_LONGLONG_T */

  overflow_ = 0u;
  samples_ = 0u;
  min_ = 0x7FFFFFFF;
  max_ = -0x8000 * 0x10000;
  sum_ = ACE_STATS_INTERNAL_OFFSET;
  sum_of_squares_ = 0u;
}

int
ACE_Stats::print_summary (const u_int precision,
                          const ACE_UINT32 scale_factor,
                          FILE *file) const
{
  if (overflow_)
    ACE_OS::fprintf (file,
                     ASYS_TEXT ("ACE_Stats::print_summary: "
                                "there was overflow!\n"));

  // Build a format string, in case the C library doesn't support %*u.
  char format[32];
  ACE_OS::sprintf (format, ASYS_TEXT ("%%d.%%0%du"), precision);

  ACE_Stats_Value u (precision);
  mean (u, scale_factor);
  char mean_string [128];
  ACE_OS::sprintf (mean_string, format, u.whole (), u.fractional ());

  ACE_Stats_Value sd (precision);
  std_dev (sd, scale_factor);
  char std_dev_string [128];
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
  char min_string [128];
  char max_string [128];
  ACE_OS::sprintf (min_string, format,
                   minimum.whole (), minimum.fractional ());
  ACE_OS::sprintf (max_string, format,
                   maximum.whole (), maximum.fractional ());

  ACE_OS::fprintf (file, ASYS_TEXT ("samples: %u (%s - %s); mean: "
                                    "%s; std dev: %s\n"),
                   samples (), min_string, max_string,
                   mean_string, std_dev_string);

  return overflow_  ?  -1  :  0;
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
    // No fractional portion is requested, so don't bother
    // calculating it.
    quotient.fractional (0);
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
    // No fractional portion is requested, so don't bother
    // calculating it.
    quotient.fractional (0);
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
