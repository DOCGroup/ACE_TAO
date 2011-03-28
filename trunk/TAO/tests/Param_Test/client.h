
//=============================================================================
/**
 *  @file    client.h
 *
 *  $Id$
 *
 *  Header file for the Param_Test client application.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_CLIENT_H
#define PARAM_TEST_CLIENT_H

#include "tao/DynamicInterface/Request.h"
#include "tao/Codeset/Codeset.h"
#include "param_testC.h"
#include "results.h"

/**
 * @class Param_Test_Client
 *
 * @brief Param_Test_Client
 *
 * This class declares an interface to run the example client for
 * Param_Test CORBA server.  All the complexity for initializing
 * the client is hidden in the class.  Just the run () interface
 * is needed. The template class does the specific work of making
 * the request of the desired data type
 */
template <class T>
class Param_Test_Client
{
public:
  typedef T TEST_OBJECT;

  // = Constructor and destructor.
  Param_Test_Client (CORBA::ORB_ptr orb,
                     Param_Test_ptr objref,
                     T *);

  ~Param_Test_Client (void);

  /// run the SII test
  int run_sii_test (void);

  /// run the DII test
  int run_dii_test (void);

private:
  /// underlying ORB
  CORBA::ORB_ptr orb_;

  /// param test object reference
  Param_Test_ptr param_test_;

  /// object doing the actual work
  TEST_OBJECT *test_object_;

  /// results
  Results results_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "client.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("client.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CLIENTS_H */
