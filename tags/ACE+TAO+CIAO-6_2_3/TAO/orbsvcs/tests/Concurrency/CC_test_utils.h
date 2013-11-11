/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    CC_test_utils.h
 *
 *  $Id$
 *
 *    This class provides a namespace for utility functions for the
 *    concurrency service test.
 *
 *
 *  @author   Torben Worm <tworm@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/CosConcurrencyControlC.h"

#ifndef _CC_TEST_UTILS_H_
#define _CC_TEST_UTILS_H_

class CC_TestUtils
{
 public:
  /// Function to return a human readable string from a lock_mode enum.
  static char *get_lock_mode_name (CosConcurrencyControl::lock_mode mode);

  /// Creates a lock set by means of th lock set factory in the concurrency
  /// service server.
  static CosConcurrencyControl::LockSet_ptr create_lock_set (void);
 private:
};

#endif /* _CC_TEST_UTILS_H_ */
