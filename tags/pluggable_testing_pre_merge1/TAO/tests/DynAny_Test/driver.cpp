// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/DynAny_Test
//
// = FILENAME
//    driver.cpp
//
// = DESCRIPTION
//    Implementation file for the driver program.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "test_dynany.h"
#include "test_dynarray.h"
#include "test_dynenum.h"
#include "test_dynsequence.h"
#include "test_dynstruct.h"
#include "test_dynunion.h"
#include "test_wrapper.h"
#include "driver.h"

int main (int argc, char* argv[])
{
  Driver driver;
  
  // initialize the driver
  if (driver.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) driver.cpp - "
                       "Driver initialization failed\n"),
                      -1);

  // run the tests
  if (driver.run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) driver.cpp - "
                       "tests failed\n"),
                      -1);
  return 0;
}

// constructor
Driver::Driver (void)
  : orb_ (0),
    test_type_ (NO_TEST)
{
}

// destructor
Driver::~Driver (void)
{
}

// initialize the driver
int
Driver::init (int argc, char* argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Retrieve the underlying ORB
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "local",
                                    ACE_TRY_ENV);

      ACE_TRY_CHECK;

      // Parse command line and verify parameters.
      if (this->parse_args (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) driver.cpp - "
                          "parse_args failed\n"),
                          -1);

    }
  ACE_CATCHANY
    {
      ACE_TRY_ENV.print_exception ("Driver::init");
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

int
Driver::parse_args (int argc, char* argv[])
{
  if (argc == 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage:  %s"
                       " -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion]"
                       "\n",
                       argv [0]),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, "t:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        if (!ACE_OS::strcmp (get_opts.optarg, "dynany"))
          this->test_type_ = TEST_DYNANY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "dynarray"))
          this->test_type_ = TEST_DYNARRAY;
        else if (!ACE_OS::strcmp (get_opts.optarg, "dynenum"))
          this->test_type_ = TEST_DYNENUM;
        else if (!ACE_OS::strcmp (get_opts.optarg, "dynsequence"))
          this->test_type_ = TEST_DYNSEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.optarg, "dynstruct"))
          this->test_type_ = TEST_DYNSTRUCT;
        else if (!ACE_OS::strcmp (get_opts.optarg, "dynunion"))
          this->test_type_ = TEST_DYNUNION;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.        
  return 0;
}

int
Driver::run (void)
{
  int retstatus = 0;

  switch (this->test_type_)
    {
      case TEST_DYNANY:
        {
          Test_Wrapper<Test_DynAny>* wrapper = 
            new Test_Wrapper<Test_DynAny> (new Test_DynAny (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNARRAY:
        {
          Test_Wrapper<Test_DynArray>* wrapper = 
            new Test_Wrapper<Test_DynArray> (new Test_DynArray (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNENUM:
        {
          Test_Wrapper<Test_DynEnum>* wrapper = 
            new Test_Wrapper<Test_DynEnum> (new Test_DynEnum (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNSEQUENCE:
        {
          Test_Wrapper<Test_DynSequence>* wrapper = 
            new Test_Wrapper<Test_DynSequence> (new Test_DynSequence (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNSTRUCT:
        {
          Test_Wrapper<Test_DynStruct>* wrapper = 
            new Test_Wrapper<Test_DynStruct> (new Test_DynStruct (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNUNION:
        {
          Test_Wrapper<Test_DynUnion>* wrapper = 
            new Test_Wrapper<Test_DynUnion> (new Test_DynUnion (this->orb_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      default:
        break;
    }

  return retstatus;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Test_Wrapper<Test_DynAny>;
template class Test_Wrapper<Test_DynArray>;
template class Test_Wrapper<Test_DynEnum>;
template class Test_Wrapper<Test_DynSequence>;
template class Test_Wrapper<Test_DynStruct>;
template class Test_Wrapper<Test_DynUnion>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Test_Wrapper<Test_DynAny>
#pragma instantiate Test_Wrapper<Test_DynArray>
#pragma instantiate Test_Wrapper<Test_DynEnum>
#pragma instantiate Test_Wrapper<Test_DynSequence>
#pragma instantiate Test_Wrapper<Test_DynStruct>
#pragma instantiate Test_Wrapper<Test_DynUnion>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

