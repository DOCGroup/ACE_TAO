// -*- c++ -*-
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    test_wrapper.cpp
//
// = DESCRIPTION
//    This file contains the implementation of the DynAny basic test manager
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#if !defined (TEST_WRAPPER_CPP)
#define TEST_WRAPPER_CPP

#include "test_wrapper.h"
#include "ace/OS.h"
#include "ace/Log_Msg.h"

// Constructor
template <class T>
Test_Wrapper<T>::Test_Wrapper (T* t)
  : test_object_ (t)
{
}

// Destructor
template <class T>
Test_Wrapper<T>::~Test_Wrapper (void)
{
  delete this->test_object_;
}

template <class T>
int
Test_Wrapper<T>::run_test (void)
{
  const char* test_name = this->test_object_->test_name ();

  ACE_DEBUG ((LM_DEBUG,
              "********************* %s *******************\n\n",
              test_name));

  if (this->test_object_->run_test () == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) test_wrapper.cpp - run_test:"
                  "run_test exception in %s",
                  test_name));

      return -1;
    }

  return 0;
}

#endif /* TEST_WRAPPER_CPP */
