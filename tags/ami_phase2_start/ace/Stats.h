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

#ifndef ACE_STATS_H
#define ACE_STATS_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

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
  // decimal digits.  The more variation in the data, and the greater
  // the data values, the smaller the precision must be to avoid
  // overflow in the standard deviation calculation.  3 might be a
  // good value, or maybe 4.  5 will probably be too large for
  // non-trivial data sets.

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

  void dump (void) const;
  // Print to stdout.

private:
  ACE_UINT32 whole_;
  // The integer portion of the value.

  ACE_UINT32 fractional_;
  // The fractional portion of the value.

  u_int precision_;
  // The number of decimal digits of precision represented by
  // <fractional_>.  Not declared const, so the only way to change it
  // is via the assignment operator.

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
  //     4) It checks for overflow of internal state.
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
  // Provide a new sample.  Returns 0 on success, -1 if it fails due
  // to running out of memory, or to rolling over of the sample count.

  ACE_UINT32 samples (void) const;
  // Access the number of samples provided so far.

  ACE_INT32 min_value (void) const;
  // Value of the minimum sample provided so far.

  ACE_INT32 max_value (void) const;
  // Value of the maximum sample provided so far.

  void mean (ACE_Stats_Value &mean,
             const ACE_UINT32 scale_factor = 1);
  // Access the mean of all samples provided so far.  The fractional
  // part is to the specified number of digits.  E.g., 3 fractional
  // digits specifies that the fractional part is in thousandths.

  int std_dev (ACE_Stats_Value &std_dev,
               const ACE_UINT32 scale_factor = 1);
  // Access the standard deviation, whole and fractional parts.  See
  // description of <mean> method for argument descriptions.

  int print_summary (const u_int precision,
                     const ACE_UINT32 scale_factor = 1,
                     FILE * = stdout) const;
  // Print summary statistics.  If scale_factor is not 1, then the
  // results are divided by it, i.e., each of the samples is scaled
  // down by it.  If internal overflow is reached with the specified
  // scale factor, it successively tries to reduce it.  Returns -1 if
  // there is overflow even with a 0 scale factor.

  void reset ();
  // Initialize internal state.

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

  void dump (void) const;
  // Print summary statistics to stdout.

private:
  u_int overflow_;
  // Internal indication of whether there has been overflow.  Contains
  // the errno corresponding to the cause of overflow.

  ACE_UINT32 number_of_samples_;
  // Number of samples.

  ACE_INT32 min_;
  // Minimum sample value.

  ACE_INT32 max_;
  // Maximum sample value.

  ACE_Unbounded_Queue <ACE_INT32> samples_;
  // The samples.
};

#if defined (__ACE_INLINE__)
# include "ace/Stats.i"
#endif /* __ACE_INLINE__ */

#endif /* ! ACE_STATS_H */
