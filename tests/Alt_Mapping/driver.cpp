
//=============================================================================
/**
 *  @file    driver.cpp
 *
 *  $Id$
 *
 *  Driver program
 *
 *
 *  @author   Jeff Parsons
 */
//=============================================================================


#include "client.h"
#include "driver.h"
#include "results.h"
#include "tests.h"
#include "ace/Get_Opt.h"

// This function runs the test (main program)
int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // get an instance of the driver object
  Driver drv;

  // initialize the driver
  if (drv.init (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) driver.cpp - "
                       "Driver initialization failed\n"),
                      -1);

  // run various tests
  if (drv.run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) driver.cpp - "
                       "tests failed\n"),
                      -1);
  return 0;
}

// constructor
Driver::Driver (void)
{
}

Driver::~Driver (void)
{
}

// initialize the driver
int
Driver::init (int argc, ACE_TCHAR **argv)
{
  // environment to track exceptions

  // retrieve the instance of Options
  Options *opt = OPTIONS::instance ();

  char exception_string[256];

  try
    {
      ACE_OS::strcpy (exception_string, "ORB Initialization");

      // Retrieve the underlying ORB
      this->orb_ = CORBA::ORB_init (argc,
                                    argv,
                                    "internet");

      // Parse command line and verify parameters.
      if (opt->parse_args (argc, argv) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) driver.cpp - "
                           "parse_args failed\n"),
                          -1);
      // Retrieve a Param_Test object reference
      ACE_OS::strcpy (exception_string,"ORB::string_to_object() failed.");

      CORBA::Object_var temp =
        this->orb_->string_to_object (opt->param_test_ior ());


      if (CORBA::is_nil (temp.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ORB::string_to_object() returned null object for IOR <%s>\n",
                           opt->param_test_ior ()),
                          -1);

      // Get the object reference
      ACE_OS::strcpy (exception_string,"Param_Test::_narrow () failed.");

      this->objref_ = Alt_Mapping::_narrow (temp.in());

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (exception_string);
      return -1;
    }

  return 0;
}

int
Driver::run (void)
{
  // serves as a factory of Param_Client objects. It is also responsible to
  // start the test

  Options *opt = OPTIONS::instance ();  // get the options object
  int retstatus = -1;

  switch (opt->test_type ())
    {
    case Options::TEST_UB_STRING:
      {
        Alt_Mapping_Client<Test_Unbounded_String> *client = new
          Alt_Mapping_Client<Test_Unbounded_String> (this->orb_.in (),
                                                    this->objref_.in(),
                                                    new Test_Unbounded_String);
        retstatus = client->run_sii_test ();

        delete client;
      }
      break;
    case Options::TEST_UB_STRUCT_SEQUENCE:
      {
        Alt_Mapping_Client<Test_Unbounded_Struct_Sequence> *client = new
          Alt_Mapping_Client<Test_Unbounded_Struct_Sequence> (
            this->orb_.in (),
            this->objref_.in(),
            new Test_Unbounded_Struct_Sequence);

        retstatus = client->run_sii_test ();

        delete client;

        Alt_Mapping_Client<Test_Unbounded_Struct_Sequence> *client2 = new
          Alt_Mapping_Client<Test_Unbounded_Struct_Sequence>
          (this->orb_.in (),
           this->objref_.in(),
           new Test_Unbounded_Struct_Sequence);

        retstatus = client2->run_sii_test ();

        delete client2;
      }
      break;
    default:
      break;
    }

  // Get in a new environment variable
  try
    {
      if (opt->shutdown ())
        {
          this->objref_->shutdown ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("during shutdown");
    }

  return retstatus;
}
