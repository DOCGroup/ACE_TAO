/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Stats.h
 *
 *  $Id$
 *
 *  @author David L. Levine
 */
//=============================================================================


#ifndef ACE_STATS_H
#define ACE_STATS_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Utils/Unbounded_Queue.h"

#ifdef ACE_SUBSET_0
#include "ace/Logging/Log_Msg.h"
#endif

#include "ace/Timer/Basic_Stats.h"

/**
 * @class ACE_Stats_Value
 *
 * @brief Helper class for ACE_Stats.
 *
 * Container struct for 64-bit signed quantity and its
 * precision.  It would be nicer to use a fixed-point class, but
 * this is sufficient.  Users typically don't need to use this
 * class directly; see ACE_Stats below.
 */
class ACE_Export ACE_Stats_Value
{
public:
  /**
   * Constructor, which requires precision in terms of number of
   * decimal digits.  The more variation in the data, and the greater
   * the data values, the smaller the precision must be to avoid
   * overflow in the standard deviation calculation.  3 might be a
   * good value, or maybe 4.  5 will probably be too large for
   * non-trivial data sets.
   */
  ACE_Stats_Value (const u_int precision);

  /// Accessor for precision.
  u_int precision (void) const;

  /// Set the whole_ field.
  void whole (const ACE_UINT32);

  /// Accessor for the whole_ field.
  ACE_UINT32 whole (void) const;

  /// Set the fractional_ field.
  void fractional (const ACE_UINT32);

  /// Accessor for the fractional_ field.
  ACE_UINT32 fractional (void) const;

  /// Calculates the maximum value of the fractional portion, given its
  /// precision.
  ACE_UINT32 fractional_field (void) const;

  /**
   * Access the value as an _unsigned_ 64 bit quantity.  It scales the
   * value up by <precision> decimal digits, so that no precision will
   * be lost.  It assumes that <whole_> is >= 0.
   */
  void scaled_value (ACE_UINT64 &) const;

  /// Print to stdout.
  void dump (void) const;

private:
  /// The integer portion of the value.
  ACE_UINT32 whole_;

  /// The fractional portion of the value.
  ACE_UINT32 fractional_;

  /**
   * The number of decimal digits of precision represented by
   * <fractional_>.  Not declared const, so the only way to change it
   * is via the assignment operator.
   */
  u_int precision_;

  ACE_UNIMPLEMENTED_FUNC (ACE_Stats_Value (void))
};

/**
 * @class ACE_Stats
 *
 * @brief Provides simple statistical analysis.
 *
 * Simple statistical analysis package.  Prominent features are:
 * -# It does not use any floating point arithmetic.
 * -# It handles positive and/or negative sample values.  The
 *    sample value type is ACE_INT32.
 * -# It uses 64 bit unsigned, but not 64 bit signed, quantities
 *    internally.
 * -# It checks for overflow of internal state.
 * -# It has no static variables of other than built-in types.
 *
 * Example usage:
 *
 * @verbatim
 * ACE_Stats stats;
 * for (u_int i = 0; i < n; ++i)
 * {
 * const ACE_UINT32 sample = ...;
 * stats.sample (sample);
 * }
 * stats.print_summary (3);
 * @endverbatim
 */
class ACE_Export ACE_Stats
{
public:
  /// Default constructor.
  ACE_Stats (void);

  /// Provide a new sample.  Returns 0 on success, -1 if it fails due
  /// to running out of memory, or to rolling over of the sample count.
  int sample (const ACE_INT32 value);

  /// Access the number of samples provided so far.
  ACE_UINT32 samples (void) const;

  /// Value of the minimum sample provided so far.
  ACE_INT32 min_value (void) const;

  /// Value of the maximum sample provided so far.
  ACE_INT32 max_value (void) const;

  /**
   * Access the mean of all samples provided so far.  The fractional
   * part is to the specified number of digits.  E.g., 3 fractional
   * digits specifies that the fractional part is in thousandths.
   */
  void mean (ACE_Stats_Value &mean,
             const ACE_UINT32 scale_factor = 1);

  /// Access the standard deviation, whole and fractional parts.  See
  /// description of <mean> method for argument descriptions.
  int std_dev (ACE_Stats_Value &std_dev,
               const ACE_UINT32 scale_factor = 1);

  /**
   * Print summary statistics.  If scale_factor is not 1, then the
   * results are divided by it, i.e., each of the samples is scaled
   * down by it.  If internal overflow is reached with the specified
   * scale factor, it successively tries to reduce it.  Returns -1 if
   * there is overflow even with a 0 scale factor.
   */
  int print_summary (const u_int precision,
                     const ACE_UINT32 scale_factor = 1,
                     FILE * = stdout) const;

  /// Initialize internal state.
  void reset (void);

  /// Utility division function, for ACE_UINT64 dividend.
  static void quotient (const ACE_UINT64 dividend,
                        const ACE_UINT32 divisor,
                        ACE_Stats_Value &quotient);

  /// Utility division function, for ACE_Stats_Value dividend.
  static void quotient (const ACE_Stats_Value &dividend,
                        const ACE_UINT32 divisor,
                        ACE_Stats_Value &quotient);

  /**
   * Sqrt function, which uses an oversimplified version of Newton's
   * method.  It's not fast, but it doesn't require floating point
   * support.
   */
  static void square_root (const ACE_UINT64 n,
                           ACE_Stats_Value &square_root);

  /// Print summary statistics to stdout.
  void dump (void) const;

private:
  /// Internal indication of whether there has been overflow.  Contains
  /// the errno corresponding to the cause of overflow.
  u_int overflow_;

  /// Number of samples.
  ACE_UINT32 number_of_samples_;

  /// Minimum sample value.
  ACE_INT32 min_;

  /// Maximum sample value.
  ACE_INT32 max_;

  /// The samples.
  ACE_Unbounded_Queue <ACE_INT32> samples_;
};

// ****************************************************************


/// A simple class to make throughput and latency analysis.
/**
 *
 * Keep the relevant information to perform throughput and latency
 * analysis, including:
 * -# Minimum, Average and Maximum latency
 * -# Jitter for the latency
 * -# Linear regression for throughput
 * -# Accumulate results from several samples to obtain aggregated
 *    results, across several threads or experiments.
 *
 * @todo The idea behind this class was to use linear regression to
 *       determine if the throughput was linear or exhibited jitter.
 *       Unfortunately it never worked quite right, so only average
 *       throughput is computed.
 */
class ACE_Export ACE_Throughput_Stats : public ACE_Basic_Stats
{
public:
  /// Constructor
  ACE_Throughput_Stats (void);

  /// Store one sample
  void sample (ACE_UINT64 throughput, ACE_UINT64 latency);

  /// Update the values to reflect the stats in @param throughput
  void accumulate (const ACE_Throughput_Stats &throughput);

  /// Print down the stats
  void dump_results (const ACE_TCHAR* msg, ACE_UINT32 scale_factor);

  /// Dump the average throughput stats.
  static void dump_throughput (const ACE_TCHAR *msg,
                               ACE_UINT32 scale_factor,
                               ACE_UINT64 elapsed_time,
                               ACE_UINT32 samples_count);
private:
  /// The last throughput measurement.
  ACE_UINT64 throughput_last_;

#if 0
  /// These are the fields that we should keep to perform linear
  /// regression
  //@{
  ///@}
  ACE_UINT64 throughput_sum_x_;
  ACE_UINT64 throughput_sum_x2_;
  ACE_UINT64 throughput_sum_y_;
  ACE_UINT64 throughput_sum_y2_;
  ACE_UINT64 throughput_sum_xy_;
#endif /* 0 */
};


#if defined (__ACE_INLINE__)
# include "ace/Utils/Stats.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ! ACE_STATS_H */
