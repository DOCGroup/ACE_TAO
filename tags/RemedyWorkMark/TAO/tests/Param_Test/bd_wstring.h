// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_wstring.h
//
// = DESCRIPTION
//     Tests bounded wide string
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#ifndef PARAM_TEST_BOUNDED_WSTRING_H
#define PARAM_TEST_BOUNDED_WSTRING_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        tests bounded wide strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Bounded_WString
{
public:
  Test_Bounded_WString (void);
  // ctor

  ~Test_Bounded_WString (void);
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

  CORBA::WChar *in_;
  // in parameter

  CORBA::WChar *inout_;
  // inout parameter

  CORBA::WChar *out_;
  // out parameter

  CORBA::WChar *ret_;
  // return value
};

#endif /* PARAM_TEST_BOUNDED_WSTRING_H */
