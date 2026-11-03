
//=============================================================================
/**
 *  @file    driver.cpp
 *
 *  Implementation file for the driver program.
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
#include <memory>

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int error_count = 0;
  try
    {
      Driver driver;

      // initialize the driver
      if (driver.init (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) driver.cpp - "
                          "Driver initialization failed\n"),
                          -1);

      // run the tests
      error_count = driver.run ();
      if (error_count != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) driver.cpp - "
                      "%d tests failed\n",
                      error_count));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught unexpected CORBA exception:");

      ++error_count;
    }

  return error_count;
}

// destructor
Driver::~Driver ()
{
  if (!CORBA::is_nil (this->orb_.in ()))
    {
      this->orb_->shutdown ();
      this->orb_->destroy ();
    }
}

// initialize the driver
int
Driver::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Retrieve the underlying ORB
      this->orb_ = CORBA::ORB_init (argc, argv, "local");


      // Parse command line and verify parameters.
      if (this->parse_args (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                          "(%N:%l) driver.cpp - "
                          "parse_args failed\n"),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Driver::init");
      return -1;
    }

  return 0;
}

int
Driver::parse_args (int argc, ACE_TCHAR *argv[])
{
  if (argc == 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "usage:  %s\n"
                       " -t [dynany|dynarray|dynenum|dynsequence|dynstruct|dynunion]\n"
                       " -d\n",
                       argv [0]),
                      -1);

  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t:d"));
  int c;
  ACE_TCHAR const* test_str {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        test_str = get_opts.opt_arg ();

        if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynany")))
          this->test_type_ = Test_Type::dynany;
        else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynarray")))
          this->test_type_ = Test_Type::dynarray;
        else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynenum")))
          this->test_type_ = Test_Type::dynenum;
        else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynsequence")))
          this->test_type_ = Test_Type::dynsequence;
        else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynstruct")))
          this->test_type_ = Test_Type::dynstruct;
        else if (!ACE_OS::strcmp (test_str, ACE_TEXT ("dynunion")))
          this->test_type_ = Test_Type::dynunion;
        else
          ACE_DEBUG ((LM_DEBUG,
                      "I don't recognize test type %s\n",
                      test_str));
        break;

      case 'd':
        this->debug_ = true;
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
Driver::run ()
{
  int error_count = 0;

  switch (this->test_type_)
    {
      case Test_Type::dynany:
        {
          Test_Wrapper<Test_DynAny> wrapper (
            std::make_unique<Test_DynAny> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      case Test_Type::dynarray:
        {
          Test_Wrapper<Test_DynArray> wrapper (
            std::make_unique<Test_DynArray> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      case Test_Type::dynenum:
        {
          Test_Wrapper<Test_DynEnum> wrapper (
            std::make_unique<Test_DynEnum> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      case Test_Type::dynsequence:
        {
          Test_Wrapper<Test_DynSequence> wrapper (
            std::make_unique<Test_DynSequence> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      case Test_Type::dynstruct:
        {
          Test_Wrapper<Test_DynStruct> wrapper (
            std::make_unique<Test_DynStruct> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      case Test_Type::dynunion:
        {
          Test_Wrapper<Test_DynUnion> wrapper (
            std::make_unique<Test_DynUnion> (this->orb_, debug_));
          error_count = wrapper.run_test ();
        }
        break;
      default:
        break;
    }

  return error_count;
}
