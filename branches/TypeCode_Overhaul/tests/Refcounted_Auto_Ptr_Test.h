// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Refcounted_Auto_Ptr_Test.h
//
// = DESCRIPTION
//    Define class needed for generating templates. IBM C++ requires this to
//    be in its own file for auto template instantiation.
//
// = AUTHOR
//    Steve Huston  <shuston@riverace.com>
//
// ============================================================================

#ifndef ACE_TESTS_REFCOUNTED_AUTO_PTR_TEST_H
#define ACE_TESTS_REFCOUNTED_AUTO_PTR_TEST_H

struct Printer
{
  Printer (const char *message);
  ~Printer (void) ;

  void print (void);

  const char *message_;
  static size_t instance_count_;
};

#endif /* ACE_TESTS_REFCOUNTED_AUTO_PTR_TEST_H */
