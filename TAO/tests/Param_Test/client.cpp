// $Id:

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    This file contains the implementation of the client-side of the
//    Param_Test application.
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (CLIENT_CPP)
#define CLIENT_CPP

#include "options.h"
#include "results.h"
#include "client.h"

// Constructor.p
template <class T>
Param_Test_Client<T>::Param_Test_Client (CORBA::ORB_ptr orb,
                                         Param_Test_ptr objref,
                                         T *t)
  : orb_ (orb),
    param_test_ (objref),
    test_object_ (t)
{
}

// destructor
template <class T>
Param_Test_Client<T>::~Param_Test_Client (void)
{
  CORBA::release (this->orb_);
  CORBA::release (this->param_test_);
  delete this->test_object_;
}

// All the individual tests
template <class T> int
Param_Test_Client<T>::run_sii_test (void)
{
  CORBA::ULong i;  // loop index
  CORBA::Environment env; // to track errors
  Options *opt = OPTIONS::instance (); // get the options
  const char *opname = this->test_object_->opname (); // operation

  // initialize parameters for the test
  this->test_object_->init_parameters ();

  // initialize call count and error count
  this->results_.call_count (0);
  this->results_.error_count (0);

  // start the timing
  this->results_.start_timer ();

  // Make the calls in a loop.
  for (i = 0; i < opt->loop_count (); i++)
    {
      this->results_.call_count (this->results_.call_count () + 1);

      // make the call
      if (this->test_object_->run_sii_test (this->param_test_, env) == -1)
        {
          this->results_.error_count (this->results_.error_count () + 1);
          env.print_exception (opname);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - run_sii_test:"
                      "run_sii_test exception in iteration %d",
                      i));
          continue;
        }
      // now check if the values returned are as expected
      if (this->test_object_->check_validity () == -1)
        {
          this->results_.error_count (this->results_.error_count () + 1);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - run_sii_test: "
                      "Invalid results in iteration %d - ",
                      i));
          this->test_object_->print_values ();
          continue;
        }
    }

  // stop the timer.
  this->results_.stop_timer ();

  // print statistics
  this->results_.print_stats (this->test_object_->opname ());
  return this->results_.error_count ()? -1:0;
}

// use DII
template <class T> int
Param_Test_Client<T>::run_dii_test (void)
{
  CORBA::ULong i;  // loop index
  CORBA::Request_ptr req; // DII request
  const char *opname = this->test_object_->opname ();
  Options *opt = OPTIONS::instance ();
  CORBA::Environment env; // environment
  CORBA::NVList_ptr nvlist;  // argument list for DII parameters
  CORBA::NVList_ptr retval; // to access teh NamedValue that stores the result

  // initialize parameters for the test
  this->test_object_->init_parameters ();

  // initialize call count and error count
  this->results_.call_count (0);
  this->results_.error_count (0);

  // start the timing
  this->results_.start_timer ();

  // Make the calls in a loop.
  for (i = 0; i < opt->loop_count (); i++)
    {
      this->results_.call_count (this->results_.call_count () + 1);

      // first create the argument list and populate it
      this->orb_->create_list (3, nvlist);
      this->orb_->create_list (1, retval);

      // add arguments and typecode for return valueto the NVList
      if (this->test_object_->add_args (nvlist, retval, env) == -1)
        {
          this->results_.error_count (this->results_.error_count () + 1);
          env.print_exception (opname);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - "
                      "Failed to add args in iteration %d",
                      i));
          continue;
        }

      // create the request
      this->param_test_->_create_request (opname,
                                          nvlist,
                                          retval->item (0),
                                          req,
                                          0,
                                          env);

      // Make the invocation, verify the result.
      req->invoke ();
      if (req->env ()->exception () != 0)
        {
          this->results_.error_count (this->results_.error_count () + 1);
          req->env ()->print_exception (opname);
          CORBA::release (req);
          continue;
        }
      // now check if the values returned are as expected
      if (this->test_object_->check_validity (req) ==  -1)
        {
          this->results_.error_count (this->results_.error_count () + 1);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - "
                      "Invalid results in run_dii_test in iteration %d",
                      i));
          this->test_object_->print_values ();
          CORBA::release (req);
          continue;
        }
      // release the request
      CORBA::release (req);
    } // for loop

  // stop the this->results_.
  this->results_.stop_timer ();

  // print statistics
  this->results_.print_stats (opname);
  return this->results_.error_count ()? -1:0;

  return 0;
}


#endif /* CLIENT_CPP */
