// $Id$
//
// Define struct used in test_position_independent_malloc.cpp - needs to be in
// a separate file for template generation on AIX IBM C++.

#ifndef __TEST_POSITION_INDEPENDENT_MALLOC_H
#define __TEST_POSITION_INDEPENDENT_MALLOC_H

// Some dummy data 
struct Dummy_Data
{
  int i1_;
  int i2_;
  int i3_;
  ACE_Based_Pointer<Dummy_Data> next_;
};

#endif /* __TEST_POSITION_INDEPENDENT_MALLOC */
