// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Framework_Component_Test.h
//
// = DESCRIPTION
//    Define class needed for generating templates. IBM C++ requires this to
//    be in its own file for auto template instantiation.
//
// = AUTHOR
//    Don Hinton <dhinton@ieee.org>
//
// ============================================================================

#ifndef ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H
#define ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H

#include "ace/Log_Msg.h"

/// This is a singleton we will use to test the ACE_Framework_Repository
class My_Singleton
{
public:
  My_Singleton (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::ctor\n")));
    }
  virtual ~My_Singleton (void)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT ("My_Singleton::dtor\n")));
    }

  static My_Singleton * instance (void);

  static void close_singleton (void);

private:
  static My_Singleton *instance_;
};

#endif /* ACE_TESTS_FRAMEWORK_COMPONENT_TEST_H */
