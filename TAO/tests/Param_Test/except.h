// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/Param_Test
//
// = FILENAME
//   except.h
//
// = DESCRIPTION
//   Tests system and user exceptions
//
// = AUTHORS
//   Carlos O'Ryan
//
// ============================================================================

#ifndef PARAM_TEST_EXCEPTION_H
#define PARAM_TEST_EXCEPTION_H

#include "param_testCli.h"

class Test_Exception
{
public:
  Test_Exception (void);
  // ctor

  ~Test_Exception (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr nvlist,
                CORBA::NVList_ptr retval,
                CORBA::Environment &env);

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref,
                       CORBA::Environment &env);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

  void dii_req_invoke (CORBA::Request *, CORBA::Environment &);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  CORBA::ULong in_;
  // in parameter

  CORBA::ULong inout_;
  // inout parameter

  CORBA::ULong out_;
  // out parameter

  CORBA::ULong ret_;
  // return value
};

#endif /* PARAM_TEST_EXCEPTION_H */
