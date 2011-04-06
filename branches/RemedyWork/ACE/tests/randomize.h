// -*- C++ -*-

// ============================================================================
/**
 * @file randomize.h
 *
 * $Id$
 *
 * Randomize the contents of an array.
 *
 * @author Ossama Othman
 *
 * @note Based on code originally found in test_config.h and Test_Output.cpp
 */
// ============================================================================

#ifndef ACE_TESTS_RANDOMIZE_H
#define ACE_TESTS_RANDOMIZE_H

#include "ace/OS_NS_stdlib.h"

#include <algorithm>
#include <functional>

namespace
{
  /**
   * @class randomize_index
   *
   * @brief Functor that randomoly swaps the order of two array
   *        elements.
   *
   * @internal Do not directly use this class.  Use the @c randomize()
   *           function template found below instead.
   *
   * This functor generates random array index using high-order bits (e.g.,
   * MAX_VAL * rand() / (RAND_MAX + 1.0)) rather than low-order ones
   * (e.g. rand() % MAX_VAL) to improve randomness.  The array element
   * reference by that index is then swapped with the array element
   * provided to the functor.
   *
   * @note The improved randomness may be overkill for this functor's
   *       intended use cases, but it is a good thing to practice/have
   *       nonetheless.
   *
   * @see Numerical Recipes in C: The Art of Scientific Computing
   *      (William H. Press, Brian P. Flannery, Saul A. Teukolsky,
   *      William T. Vetterling; New York: Cambridge University Press,
   *      1992 (2nd ed., p. 277).
   */
  template<typename T>
  class randomize_element
    : public std::unary_function<T &, void>
  {
  public:

    randomize_element (T * array, size_t size, unsigned int seed)
      : array_ (array)
      , coefficient_ (static_cast<double> (size) / (RAND_MAX + 1.0f))
      , seed_ (seed)
    {
    }

    void
    operator() (T & value)    // We need this to be a reference!
    {
      size_t const index =
        static_cast<size_t> (
          this->coefficient_ * ACE_OS::rand_r (&this->seed_));

      // Swap rather than assign so that we don't lose the original
      // value.
      std::swap (value, this->array_[index]);
    }

  private:

    // The array.
    T * const array_;

    // Factor out the constant coefficient.
    double const coefficient_;

    // Random number generator seed value.
    unsigned int seed_;

  };

  /**
   * @fn randomize
   *
   * @brief Randomize the order of elements in an array.
   *
   * @param array The array whose elements will be reordered
   *              randomly.
   * @param size  The number of elements in the array.
   * @param seed  The random number generator seed value.  A default
   *              value is provided so that the generated random
                  number order may be "repeatable" if so desired.
   *
   * This function template randomizes the order of elements in an
   * array of any type.  It is also reentrant (unless the array itself
   * is accessed by multiple threads).
   * @par
   * Sample usage:
   * @par
   * \verbatim
     static size_t const NUM_TIMES = 100;
     ACE_Time_Value times[NUM_TIMES] = { ... };
     randomize (times,
                NUM_TIMES,
                static_cast<unsigned int> (ACE_OS::time (0L)));
    \endverbatim
   */
  template<typename T>
  void
  randomize (T * array, size_t size, unsigned int seed = 0)
  {
    // Randomize all elements of the array.
    std::for_each (array,
                   array + size,
                   randomize_element<T> (array, size, seed));
  }
}

#endif  /* !ACE_TESTS_RANDOMIZE_H */
