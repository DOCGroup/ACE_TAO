// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    driver.cpp
//
// = DESCRIPTION
//    Driver program
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#include "options.h"
#include "results.h"
#include "client.h"
#include "tests.h"
#include "driver.h"

// This function runs the test (main program)
int
main (int argc, char **argv)
{
  // get an instance of the driver object
  Driver *drv = DRIVER::instance ();

  // initialize the driver
  if (drv->init (argc, argv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) driver.cpp - "
                         "Driver initialization failed\n"),
                        -1);
    }

  // run various tests
  if (drv->run () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) driver.cpp - "
                         "tests failed\n"),
                        -1);
    }
  return 0;
}

// constructor
Driver::Driver (void)
  : orb_ptr_ (0)
{
}

Driver::~Driver (void)
{
}

// initialize the driver
int
Driver::init (int argc, char **argv)
{
  // environment to track exceptions
  CORBA::Environment env;

  // retrieve the instance of Options
  Options *opt = OPTIONS::instance ();

  // Retrieve the underlying ORB
  this->orb_ptr_ = CORBA::ORB_init (argc,
                                    argv,
                                    "internet",
                                    env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB initialization");
      return -1;
    }

  // Parse command line and verify parameters.
  if (opt->parse_args (argc, argv) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) driver.cpp - "
                         "parse_args failed\n"),
                        -1);
    }

  // Retrieve a Param_Test object reference
  CORBA::Object_var temp =
    this->orb_ptr_->string_to_object (opt->param_test_ior (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("ORB::string_to_object() failed.");
      return -1;
    }

  if (CORBA::is_nil (temp.in()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ORB::string_to_object() returned null object for IOR <%s>\n",
                         opt->param_test_ior ()),
                        -1);
    }

  
  this->objref_ = Param_Test::_narrow (temp.in(), env);
  if (env.exception () != 0)
    {
      env.print_exception ("Param_Test::_narrow failed");
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
    case Options::TEST_SHORT:
      {
        Param_Test_Client<Test_Short> *client = new
          Param_Test_Client<Test_Short> (this->orb_ptr_,
                                         this->objref_.in(),
                                         new Test_Short);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_UNBOUNDED_STRING:
      {
        Param_Test_Client<Test_Unbounded_String> *client = new
          Param_Test_Client<Test_Unbounded_String> (this->orb_ptr_,
                                                    this->objref_.in(),
                                                    new Test_Unbounded_String);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_FIXED_STRUCT:
      {
        Param_Test_Client<Test_Fixed_Struct> *client = new
          Param_Test_Client<Test_Fixed_Struct> (this->orb_ptr_,
                                                this->objref_.in(),
                                                new Test_Fixed_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_STRING_SEQUENCE:
      {
        Param_Test_Client<Test_String_Sequence> *client = new
          Param_Test_Client<Test_String_Sequence> (this->orb_ptr_,
                                                   this->objref_.in(),
                                                   new Test_String_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_VAR_STRUCT:
      {
        Param_Test_Client<Test_Var_Struct> *client = new
          Param_Test_Client<Test_Var_Struct> (this->orb_ptr_,
                                              this->objref_.in(),
                                              new Test_Var_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_NESTED_STRUCT:
      {
        Param_Test_Client<Test_Nested_Struct> *client = new
          Param_Test_Client<Test_Nested_Struct> (this->orb_ptr_,
                                                 this->objref_.in(),
                                                 new Test_Nested_Struct);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_STRUCT_SEQUENCE:
      {
        Param_Test_Client<Test_Struct_Sequence> *client = new
          Param_Test_Client<Test_Struct_Sequence> (this->orb_ptr_,
                                                 this->objref_.in(),
                                                 new Test_Struct_Sequence);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    case Options::TEST_OBJREF:
      {
        Param_Test_Client<Test_ObjRef> *client = new
          Param_Test_Client<Test_ObjRef> (this->orb_ptr_,
                                          this->objref_.in(),
                                          new Test_ObjRef);
        if (opt->invoke_type () == Options::SII)
          retstatus = client->run_sii_test ();
        else
          retstatus = client->run_dii_test ();
        delete client;
      }
      break;
    default:
      break;
    }
  return retstatus;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Driver, ACE_SYNCH_RECURSIVE_MUTEX>;
template class Param_Test_Client<Test_Short>;
template class Param_Test_Client<Test_Unbounded_String>;
template class Param_Test_Client<Test_Fixed_Struct>;
template class Param_Test_Client<Test_String_Sequence>;
template class Param_Test_Client<Test_Var_Struct>;
template class Param_Test_Client<Test_Nested_Struct>;
template class Param_Test_Client<Test_Struct_Sequence>;
template class Param_Test_Client<Test_ObjRef>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Driver, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate Param_Test_Client<Test_Short>
#pragma instantiate Param_Test_Client<Test_Unbounded_String>
#pragma instantiate Param_Test_Client<Test_Fixed_Struct>
#pragma instantiate Param_Test_Client<Test_String_Sequence>
#pragma instantiate Param_Test_Client<Test_Var_Struct>
#pragma instantiate Param_Test_Client<Test_Nested_Struct>
#pragma instantiate Param_Test_Client<Test_Struct_Sequence>
#pragma instantiate Param_Test_Client<Test_ObjRef>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
