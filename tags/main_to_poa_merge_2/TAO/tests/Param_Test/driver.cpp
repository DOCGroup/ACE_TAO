// $Id:

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
}

// constructor
Driver::Driver (void)
  : orb_ptr_ (0),
    objref_ (Param_Test::_nil ())
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
  this->objref_ = Param_Test::_bind (opt->hostname (),
                                     opt->portnum (),
                                     opt->param_test_key (),
                                     env);

  if (env.exception () != 0)
    {
      env.print_exception ("Param_Test::_bind");
      return -1;
    }

  if (CORBA::is_nil (this->objref_))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         " _bind returned null object for "
                         "key (%s), host (%s), port (%d)\n",
                         opt->param_test_key (),
                         opt->hostname (),
                         opt->portnum ()),
                        -1);
    }

  return 0;
}

int
Driver::run (void)
{
  // serves as a factory of Param_Client objects. It is also responsible to
  // start the test

  Options *opt = OPTIONS::instance ();  // get the options object

  switch (opt->test_type ())
    {
    case Options::TEST_SHORT:
      {
        Param_Test_Client<Test_Short> *client = new
          Param_Test_Client<Test_Short> (this->orb_ptr_,
                                         this->objref_,
                                         new Test_Short);
        if (opt->invoke_type () == Options::SII)
          return client->run_sii_test ();
        else
          return client->run_dii_test ();
      }
      break;
    }
  return -1; // failure of some kind
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<Driver, ACE_SYNCH_RECURSIVE_MUTEX>;
template class Param_Test_Client<Test_Short>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<Driver, ACE_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate Param_Test_Client<Test_Short>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
