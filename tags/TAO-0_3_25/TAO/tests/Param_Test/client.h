// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//    Header file for the Param_Test client application.
//
// = AUTHORS
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_CLIENT_H
#define PARAM_TEST_CLIENT_H

#include "param_testCli.h"
#include "results.h"

template <class T>
class Param_Test_Client
{
  // = TITLE
  //    Param_Test_Client
  //
  // = DESCRIPTION
  //    This class declares an interface to run the example client for
  //    Param_Test CORBA server.  All the complexity for initializing
  //    the client is hidden in the class.  Just the run () interface
  //    is needed. The template class does the specific work of making
  //    the request of the desired data type
public:
  typedef T TEST_OBJECT;

  // = Constructor and destructor.
  Param_Test_Client (CORBA::ORB_ptr orb,
                     Param_Test_ptr objref,
                     T *);

  ~Param_Test_Client (void);

  int run_sii_test (void);
  // run the SII test

  int run_dii_test (void);
  // run the DII test

private:
  CORBA::ORB_ptr orb_;
  // underlying ORB

  Param_Test_ptr param_test_;
  // param test object reference

  TEST_OBJECT *test_object_;
  // object doing the actual work

  Results results_;
  // results
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "client.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("client.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CLIENTS_H */
