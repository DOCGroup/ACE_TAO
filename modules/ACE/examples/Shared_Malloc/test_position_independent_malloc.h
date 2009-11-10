// $Id$
//
// Define struct used in test_position_independent_malloc.cpp - needs
// to be in a separate file for template generation on AIX IBM C++.

#ifndef __TEST_POSITION_INDEPENDENT_MALLOC_H
#define __TEST_POSITION_INDEPENDENT_MALLOC_H

#include "ace/Based_Pointer_T.h"

// Some test data.
struct Long_Test
{
  ACE_Based_Pointer_Basic<long> bpl_;
  long array_[10];
};

// Some more test data.
struct Test_Data
{
  int i1_;
  int i2_;
  int i3_;
  ACE_Based_Pointer<Test_Data> next_;
  ACE_Based_Pointer<Long_Test> long_test_;
};

#endif /* __TEST_POSITION_INDEPENDENT_MALLOC */
