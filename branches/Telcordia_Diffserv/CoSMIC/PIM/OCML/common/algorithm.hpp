/* -*- C++ -*- */

/**
 * @file XercesHelpers.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _COMMON_ALGORITHM_HPP_
#define _COMMON_ALGORITHM_HPP_

namespace common
{

  /// Checks if all of the elements in [begin, end) satisfies the f.
  template <typename InputIterator, typename Predicate>
  bool satisfies_each(InputIterator begin, InputIterator end, Predicate f);

} // namespace common

#include "algorithm.tpp"

#endif // _COMMON_ALGORITHM_HPP_
