//=============================================================================
/**
 *  @file    Malloc_Test.h
 *
 *  $Id$
 *
 *  This class gets its own header file to work around AIX C++
 *  compiler "features" related to template instantiation...  It is
 *  only used by Malloc_Test.cpp.
 *
 *
 *  @author   Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_MALLOC_TEST_H
#define ACE_TESTS_MALLOC_TEST_H

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
  double d1_;
  ACE_Based_Pointer<Test_Data> next_;
  ACE_Based_Pointer<Long_Test> long_test_;
};

#endif /* ACE_TESTS_MALLOC_TEST_H */
