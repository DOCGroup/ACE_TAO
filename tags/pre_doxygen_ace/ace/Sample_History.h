// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Sample_History.h
//
// = AUTHOR
//    Carlos O'Ryan <coryan@uci.edu>
//
// ============================================================================

#ifndef ACE_SAMPLE_HISTORY_H
#define ACE_SAMPLE_HISTORY_H
#include "ace/pre.h"

#include "ace/config-all.h"
#include "ace/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Basic_Stats;

/// Save multiple samples in an array
/**
 * Save multiple samples (usually latency numbers), into an array, and
 * later print them in several formats.
 */
class ACE_Export ACE_Sample_History
{
public:
  /// Constructor
  /**
   * The number of samples is pre-allocated, and cannot changes once
   * the class is initialized.
   */
  ACE_Sample_History (size_t max_samples);

  /// Destructor
  ~ACE_Sample_History (void);

  /// Record one sample.
  /**
   * Return 0 on success, -1 if the sample could not be stored
   */
  int sample (ACE_UINT64 value);

  /// Dump all the samples
  /**
   * Prints out all the samples, using @param msg as a prefix for each
   * message.  
   */
  void dump_samples (const ACE_TCHAR *msg,
                     ACE_UINT32 scale_factor) const;

  /// Collect the summary for all the samples
  void collect_basic_stats (ACE_Basic_Stats &) const;

private:
  /// The maximum number of samples
  size_t max_samples_;

  /// The current number of samples
  size_t sample_count_;

  /// The samples
  ACE_UINT64 *samples_;
};

#if defined (__ACE_INLINE__)
#include "ace/Sample_History.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SAMPLE_HISTORY_H */
