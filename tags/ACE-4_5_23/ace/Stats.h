/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Stats.h
//
// = AUTHORS
//    David L. Levine
//
// ============================================================================

#if !defined (ACE_STATS_H)
#define ACE_STATS_H

#include "ace/ACE.h"

class ACE_Export ACE_Stats_Value
{
  // = TITLE
  //     Helper class for ACE_Stats.
  //
  // = DESCRIPTION
  //     Container struct for 64-bit signed quantity and its
  //     precision.  It would be nicer to use a fixed-point class, but
  //     this is sufficient.  Users typically don't need to use this
  //     class directly; see ACE_Stats below.
public:
  ACE_Stats_Value (const u_int precision);
  // Constructor, which requires precision in terms of number of
  // decimal digits.  Don't use more than 9!

  u_int precision (void) const;
  // Accessor for precision.

  void whole (const ACE_UINT32);
  // Set the whole_ field.

  ACE_UINT32 whole (void) const;
  // Accessor for the whole_ field.

  void fractional (const ACE_UINT32);
  // Set the fractional_ field.

  ACE_UINT32 fractional (void) const;
  // Accessor for the fractional_ field.

  ACE_UINT32 fractional_field (void) const;
  // Calculates the maximum value of the fractional portion, given its
  // precision.

  void scaled_value (ACE_UINT64 &) const;
  // Access the value as an _unsigned_ 64 bit quantity.  It scales the
  // value up by <precision> decimal digits, so that no precision will
  // be lost.  It assumes that <whole_> is >= 0.

private:
  ACE_UINT32 whole_;
  // The integer portion of the value.

  ACE_UINT32 fractional_;
  // The fractional portion of the value.

  const u_int precision_;
  // The number of decimal digits of precision represented by
  // <fractional_>.

  ACE_UNIMPLEMENTED_FUNC (ACE_Stats_Value (void))
};

class ACE_Export ACE_Stats
{
  // = TITLE
  //     Provides simple statistical analysis.
  //
  // = DESCRIPTION
  //     Simple statistical analysis package.  Prominent features are:
  //     1) It does not use any floating point arithmetic.
  //     2) It handles positive and/or negative sample values.  The
  //        sample value type is ACE_INT32.
  //     3) It uses 64 bit unsigned, but not 64 bit signed, quantities
  //        internally.
  //     4) It checks for overflow of internal state variables.
  //     5) It has no static variables of other than built-in types.
  //
  //     Example usage:
  //       ACE_Stats stats;
  //       for (u_int i = 0; i < n; ++i)
  //         {
  //           const ACE_UINT32 sample = /* ... */;
  //           stats.sample (sample);
  //         }
  //       stats.print_summary (3);
public:
  ACE_Stats (void);
  // Default constructor.

  int sample (const ACE_INT32 value);
  // Provide a new sample.  Returns 0 on success, -1 if internal overflow.
  // If internal overflow is reached on this or any previous call, the
  // state is not changed.  Therefore, print_summary () can still be used.

  ACE_UINT32 samples (void) const;
  // Access the number of samples provided so far.

  ACE_INT32 min_value (void) const;
  // Value of the minimum sample provided so far.

  ACE_INT32 max_value (void) const;
  // Value of the maximum sample provided so far.

  void mean (ACE_Stats_Value &mean,
             const ACE_UINT32 scale_factor = 1) const;
  // Access the mean of all samples provided so far.  The fractional
  // part is to the specified number of digits.  E.g., 3 fractional
  // digits specifies that fractional part is in thousandths.

  void std_dev (ACE_Stats_Value &std_dev,
                const ACE_UINT32 scale_factor = 1) const;
  // Access the standard deviation, whole and fractional parts.  See
  // description of <mean> method for argument descriptions.

  int print_summary (const u_int precision,
                     const ACE_UINT32 scale_factor = 1,
                     FILE * = stdout) const;
  // Print summary statistics.  If scale_factor is not 1, then the
  // results are divided by it, i.e., each of the samples is scaled
  // down by it.  Returns -1 if internal overflow had been reached.
  // The statistics will still be valid, but only for the samples
  // received prior to the overflow.

  void reset ();
  // Initialize internal state.

  void dump (void) const;
  // Print summary statictics to stdout.

  static void quotient (const ACE_UINT64 dividend,
                        const ACE_UINT32 divisor,
                        ACE_Stats_Value &quotient);
  // Utility division function, for ACE_UINT64 dividend.

  static void quotient (const ACE_Stats_Value &dividend,
                        const ACE_UINT32 divisor,
                        ACE_Stats_Value &quotient);
  // Utility division function, for ACE_Stats_Value dividend.

  static void square_root (const ACE_UINT64 n,
                           ACE_Stats_Value &square_root);
  // Sqrt function, which uses an oversimplified version of Newton's
  // method.  It's not fast, but it doesn't require floating point
  // support.

private:
  u_int overflow_;
  // Internal indication of whether there has been overflow.

  ACE_UINT32 samples_;
  // Number of samples.

  ACE_INT32 min_;
  // Minimum sample value.

  ACE_INT32 max_;
  // Maximum sample value.

  ACE_UINT64 sum_;
  // Running sum.

  ACE_UINT64 sum_of_squares_;
  // Running sum of squares.
};

#if defined (__ACE_INLINE__)
#include "ace/Stats.i"
#endif /* __ACE_INLINE__ */

#endif /* ! ACE_STATS_H */
