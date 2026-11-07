
//=============================================================================
/**
 *  @file    client.h
 *
 *  Header file for the Param_Test client application.
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
#include <memory>

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
  using TEST_OBJECT = T;

  // = Constructor and destructor.
  /// Takes ownership of the test object.
  Param_Test_Client (CORBA::ORB_ptr orb,
                     Param_Test_ptr objref,
                     T *test_object);

  ~Param_Test_Client () = default;
  Param_Test_Client (Param_Test_Client const &) = delete;
  Param_Test_Client (Param_Test_Client &&) = delete;
  Param_Test_Client &operator= (Param_Test_Client const &) = delete;
  Param_Test_Client &operator= (Param_Test_Client &&) = delete;

  /// run the SII test
  int run_sii_test ();

  /// run the DII test
  int run_dii_test ();

private:
  /// underlying ORB
  CORBA::ORB_ptr orb_;

  /// param test object reference
  Param_Test_ptr param_test_;

  /// object doing the actual work
  std::unique_ptr<TEST_OBJECT> test_object_;

  /// results
  Results results_;
};

#include "client.cpp"
#endif /* CLIENTS_H */
