// $Id$

// ============================================================================
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION
//    This file contains the implementation of the client-side of the
//    Alt_Mapping application.
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#ifndef CLIENT_CPP
#define CLIENT_CPP

#include "options.h"
#include "results.h"
#include "client.h"

// Constructor.p
template <class T>
Alt_Mapping_Client<T>::Alt_Mapping_Client (CORBA::ORB_ptr orb,
                                           Alt_Mapping_ptr objref,
                                           T *t)
  : orb_ (orb),
    alt_mapping_test_ (objref),
    test_object_ (t)
{
}

// destructor
template <class T>
Alt_Mapping_Client<T>::~Alt_Mapping_Client (void)
{
  delete this->test_object_;
}

// All the individual tests.
template <class T> int
Alt_Mapping_Client<T>::run_sii_test (void)
{
  CORBA::ULong i = 0;  // loop index
  Options *opt = OPTIONS::instance (); // get the options
  const char *opname = this->test_object_->opname (); // operation

  if (opt->debug ())
    ACE_DEBUG ((LM_DEBUG,
                "********** %s SII *********\n",
                opname));

  // Initialize call count and error count.
  this->results_.call_count (0);
  this->results_.error_count (0);
  this->results_.iterations (opt->loop_count ());

  // Declare the Env
  // Initialize parameters for the test.
  int check = this->test_object_->init_parameters (this->alt_mapping_test_);

  if (check == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) client.cpp - run_sii_test:"
                         "init_parameters failed for opname - %s",
                         opname),
                        -1);
    }


  // Make the calls in a loop.
  for (i = 0; i < opt->loop_count (); i++)
    {
      try
        {
          this->results_.call_count (this->results_.call_count () + 1);
          if (opt->debug ())
            {
              ACE_DEBUG ((LM_DEBUG, "\n****** Before call values *****\n"));
              this->test_object_->print_values ();
            }

          // start the timing
          this->results_.start_timer ();

          // make the call
          this->test_object_->run_sii_test (this->alt_mapping_test_);

          // stop the timer.
          this->results_.stop_timer ();

          // now check if the values returned are as expected
          if (opt->debug ())
            {
              ACE_DEBUG ((LM_DEBUG, "\n****** After call values *****\n"));
              this->test_object_->print_values ();
            }
        }
      catch (const CORBA::Exception& ex)
        {

          this->results_.error_count (this->results_.error_count () + 1);
          ex._tao_print_exception (opname);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - run_sii_test:"
                      "run_sii_test exception in iteration %d",
                      i));
          goto loop_around;

        }

      if (!this->test_object_->check_validity ())
        {
          this->results_.error_count (this->results_.error_count () + 1);
          ACE_ERROR ((LM_ERROR,
                      "(%N:%l) client.cpp - run_sii_test: "
                      "Invalid results in iteration %d\n",
                      i));
          continue;
        }
      // reset parameters for the test.
      if (this->test_object_->reset_parameters () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) client.cpp - run_sii_test:"
                           "init_parameters failed for opname - %s",
                           opname), -1);
    loop_around: continue;
    }

  // print statistics
  this->results_.print_stats ();

  if (this->results_.error_count () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "********** Error running %s SII *********\n",
                  opname));
    }
  else if (opt->debug ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "********** Finished running %s SII *********\n",
                  opname));
    }

  return this->results_.error_count ()? -1 : 0;
}

#endif /* CLIENT_CPP */
