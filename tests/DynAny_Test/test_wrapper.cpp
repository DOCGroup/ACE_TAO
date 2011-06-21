//=============================================================================
/**
 *  @file    test_wrapper.cpp
 *
 *  $Id$
 *
 *  This file contains the implementation of the DynAny basic test manager
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_WRAPPER_CPP)
#define TEST_WRAPPER_CPP

#include "test_wrapper.h"
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
              "********************* %C *******************\n\n",
              test_name));

  return this->test_object_->run_test ();
}

#endif /* TEST_WRAPPER_CPP */
