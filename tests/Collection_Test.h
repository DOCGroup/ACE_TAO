// -*- C++ -*-


//=============================================================================
/**
 *  @file    Collection_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_COLLECTION_TEST_H
#define ACE_TESTS_COLLECTION_TEST_H

typedef void (*deletion_func)(void* p);

struct UglyThing
{
  void *alloc_;
  deletion_func dfunc_;

  UglyThing (void* alloc = 0, deletion_func dfunc = 0);
  bool operator== (const UglyThing& r) const;
};

#endif /* ACE_TESTS_COLLECTION_TEST_H */
