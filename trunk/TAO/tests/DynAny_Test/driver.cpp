
//=============================================================================
/**
 *  @file    driver.cpp
 *
 *  $Id$
 *
 *  Implementation file for the driver program.
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#include "driver.h"
#include "test_dynany.h"
#include "test_dynarray.h"
#include "test_dynenum.h"
#include "test_dynsequence.h"
#include "test_dynstruct.h"
#include "test_dynunion.h"
#include "test_wrapper.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"

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
  : test_type_ (NO_TEST),
    debug_ (0)
{
}

// destructor
Driver::~Driver (void)
{
  if (this->orb_.in () != 0)
    {
      this->orb_->shutdown ();
      this->orb_->destroy ();
    }
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
                                    "local"
                                    ACE_ENV_ARG_PARAMETER);

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
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Driver::init");
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

  ACE_Get_Opt get_opts (argc, argv, "t:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynany"))
          this->test_type_ = TEST_DYNANY;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynarray"))
          this->test_type_ = TEST_DYNARRAY;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynenum"))
          this->test_type_ = TEST_DYNENUM;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynsequence"))
          this->test_type_ = TEST_DYNSEQUENCE;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynstruct"))
          this->test_type_ = TEST_DYNSTRUCT;
        else if (!ACE_OS::strcmp (get_opts.opt_arg (), "dynunion"))
          this->test_type_ = TEST_DYNUNION;
        break;

      case 'd':
        this->debug_ = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion]"
                           " -d for debug"
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
            new Test_Wrapper<Test_DynAny> (new Test_DynAny (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNARRAY:
        {
          Test_Wrapper<Test_DynArray>* wrapper =
            new Test_Wrapper<Test_DynArray> (new Test_DynArray (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNENUM:
        {
          Test_Wrapper<Test_DynEnum>* wrapper =
            new Test_Wrapper<Test_DynEnum> (new Test_DynEnum (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNSEQUENCE:
        {
          Test_Wrapper<Test_DynSequence>* wrapper =
            new Test_Wrapper<Test_DynSequence> (new Test_DynSequence (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNSTRUCT:
        {
          Test_Wrapper<Test_DynStruct>* wrapper =
            new Test_Wrapper<Test_DynStruct> (new Test_DynStruct (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      case TEST_DYNUNION:
        {
          Test_Wrapper<Test_DynUnion>* wrapper =
            new Test_Wrapper<Test_DynUnion> (new Test_DynUnion (this->orb_, debug_));
          retstatus = wrapper->run_test ();
          delete wrapper;
        }
        break;
      default:
        break;
    }

  return retstatus;
}
