/* -*- C++ -*- */
/**
 *  @file Swap.h
 *
 *  $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef ACE_SWAP_H
#define ACE_SWAP_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class ACE_Swap
 *
 * @brief Defines a function to swap two variables.
 *
 */
template<class T> class ACE_Swap
{
public:
  /// Swap two variables
  static void swap (T &lhs, T &rhs);
};

#if defined (__ACE_INLINE__)
#include "ace/Swap.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Swap.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Swap.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_FUNCTOR_H */
