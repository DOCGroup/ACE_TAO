
//=============================================================================
/**
 *  @file   except.h
 *
 *  $Id$
 *
 * Tests system and user exceptions
 *
 *
 *  @author Carlos O'Ryan
 */
//=============================================================================


#ifndef PARAM_TEST_EXCEPTION_H
#define PARAM_TEST_EXCEPTION_H

#include "param_testC.h"

class Test_Exception
{
public:
  /// ctor
  Test_Exception (void);

  /// dtor
  ~Test_Exception (void);

  /// run the SII test
  int run_sii_test (Param_Test_ptr objref);

  /// return operation name
  const char *opname (void) const;

  /// set values for parameters
  int init_parameters (Param_Test_ptr objref);

  /// reset values for CORBA
  int reset_parameters (void);

  /// check if results are valid
  CORBA::Boolean check_validity (void);

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values (void);

  /// invoke DII request with appropriate exception handling.
  void dii_req_invoke (CORBA::Request *);

private:
  /// operation name
  char *opname_;

  /// in parameter
  CORBA::ULong in_;

  /// inout parameter
  CORBA::ULong inout_;

  /// out parameter
  CORBA::ULong out_;

  /// return value
  CORBA::ULong ret_;

  /// Current iterations.
  CORBA::ULong iterations_;
};

#endif /* PARAM_TEST_EXCEPTION_H */
