// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    short.h
//
// = DESCRIPTION
//     Tests shorts
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_SHORT_H
#define PARAM_TEST_SHORT_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test shorts
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Short
{
public:
  Test_Short (void);
  // ctor

  ~Test_Short (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref);
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

  CORBA::Short in_;
  // in parameter

  CORBA::Short inout_;
  // inout parameter

  CORBA::Short out_;
  // out parameter

  CORBA::Short ret_;
  // return value
};

#endif /* PARAM_TEST_SHORT_H */
