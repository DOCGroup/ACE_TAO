// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ulonglong.h
//
// = DESCRIPTION
//     Tests 64bit unsigned longs.
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_ULONGLONG_H
#define PARAM_TEST_ULONGLONG_H

#include "param_testCli.h"

class Test_ULongLong
{
public:
  Test_ULongLong (void);
  // ctor

  ~Test_ULongLong (void);
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

  void dii_req_invoke (CORBA::Request *);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  CORBA::ULongLong in_;
  // in parameter

  CORBA::ULongLong inout_;
  // inout parameter

  CORBA::ULongLong out_;
  // out parameter

  CORBA::ULongLong ret_;
  // return value
};

#endif /* PARAM_TEST_ULONGLONG_H */
