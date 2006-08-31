// -*- C++ -*-

//=============================================================================
/**
 * @file    Counter.h
 *
 * $Id$
 *
 * @author James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_EVENT_COUNTER_H_
#define _CUTS_EVENT_COUNTER_H_

#include "cuts/config.h"
#include "ace/Thread_Mutex.h"

//=============================================================================
/**
 * @class CUTS_Counter
 *
 * This class is a simple thread-safe counting class. It can only increment
 * by a value of 1. It also has a reset method that allows the count to be
 * reset to 0.
 */
//=============================================================================

class CUTS_Counter
{
public:
  /// Constructor.
  CUTS_Counter (void);

  /// Destructor.
  virtual ~CUTS_Counter (void);

  /// Reset the event count to zero events.
  virtual void reset (void);

  /// Increment the number of events counted.
  virtual void increment (void);

  /// Get the current count for the counter.
  size_t count (void);

protected:
  /// Reset value for the event counter.
  size_t count_;

  /// Locking mechanism to prevent data corruption.
  ACE_Thread_Mutex lock_;
};

//=============================================================================
/**
 * @class CUTS_Bounded_Counter
 *
 * This class is a bounded version of the defualt counter. A bounded counter
 * is one that has maximum value that is cannot exceed. Once the count 
 * reaches that value it is considered full.
 */
//=============================================================================

class CUTS_Bounded_Counter :
  public CUTS_Counter
{
public:
  /// Constructor.
  CUTS_Bounded_Counter (size_t max_count);

  /// Destructor.
  virtual ~CUTS_Bounded_Counter (void);

  /// Determine if the counter is full. The counter is considered
  /// full if it has reached max count.
  bool is_full (void) const;

  /**
   * Increment the number of <counts_> by one. If the <max_count>
   * has been reached, then this will not increment the number of
   * counts.
   */
  virtual void increment (void);

  /// Reset the counter.
  virtual void reset (void);

protected:
  /// The maximum count for the counter.
  size_t max_count_;

  /// Contains the <full> state of the counter.
  bool full_;
};

#if defined (__CUTS_INLINE__)
# include "cuts/Counter.inl"
#endif

#endif  // !defined _CUTS_EVENT_COUNTER_H_
