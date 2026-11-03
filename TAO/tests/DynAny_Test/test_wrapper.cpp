//=============================================================================
/**
 *  @file    test_wrapper.cpp
 *
 *  This file contains the implementation of the DynAny basic test manager
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#if !defined (TEST_WRAPPER_CPP)
#define TEST_WRAPPER_CPP

#include "test_wrapper.h"
#include "ace/Log_Msg.h"

template <class T>
int
Test_Wrapper<T>::run_test ()
{
  char const* test_name = this->test_object_->test_name ();

  ACE_DEBUG ((LM_DEBUG,
              "********************* %C *******************\n\n",
              test_name));

  return this->test_object_->run_test ();
}

#endif /* TEST_WRAPPER_CPP */
