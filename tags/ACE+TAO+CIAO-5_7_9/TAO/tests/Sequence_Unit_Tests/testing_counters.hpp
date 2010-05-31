#ifndef guard_testing_counters_hpp
#define guard_testing_counters_hpp
/**
 * @file
 *
 * @brief Some unit tests need to count how many times a function is
 * called.   Here we implement some simple helper classes for that
 * purpose.
 *
 * $Id$
 *
 * @author Carlos O'Ryan
 */

#include "testing_exception.hpp"
#include "ace/Copy_Disabled.h"

#include <iostream>

/**
 * @brief Used to count how many times a function gets called.  The
 *        unit test should create one instance per function.
 */
class call_counter
{
public:
  inline call_counter()
    : count_(0)
    , failure_countdown_(0)
  {}

  inline long current_count() const
  {
    return count_;
  }

  inline void failure_countdown(long countdown)
  {
    failure_countdown_ = countdown;
  }

  inline void operator()()
  {
    ++count_;
    if (--failure_countdown_ == 0)
    {
      throw testing_exception();
    }
  }

private:
  long count_;
  long failure_countdown_;
};

/**
 * @brief Used to detect if a testing_counter is "called" the right
 *        number of times.
 */
class expected_calls
  : private ACE_Copy_Disabled
{
public:
  inline expected_calls(call_counter const & counter)
    : current_count_(counter.current_count())
    , previous_count_(counter.current_count())
    , counter_(counter)
  { }

  inline bool expect(long n)
  {
    reset();
    return (previous_count_ + n == current_count_);
  }

  inline void reset()
  {
    previous_count_ = current_count_;
    current_count_ = counter_.current_count();
  }

  inline long current_count() const
  {
    return current_count_;
  }

  inline long previous_count() const
  {
    return previous_count_;
  }

private:
  long current_count_;
  long previous_count_;
  call_counter const & counter_;
};

inline std::ostream & operator<<(std::ostream & os, expected_calls const & x)
{
  return os << "current=" << x.current_count()
            << ",previous=" << x.previous_count();
}

#endif // guard_testing_counters_hpp
