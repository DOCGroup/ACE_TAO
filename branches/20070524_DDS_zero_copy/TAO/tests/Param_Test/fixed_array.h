// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    fixed_array.h
//
// = DESCRIPTION
//     Tests fixed size array
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_FIXED_ARRAY_H
#define PARAM_TEST_FIXED_ARRAY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed arrays
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Fixed_Array
{
public:
  Test_Fixed_Array (void);
  // ctor

  ~Test_Fixed_Array (void);
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
  CORBA::Boolean compare (const Param_Test::Fixed_Array_slice *a1,
                          const Param_Test::Fixed_Array_slice *a2);
  // compare arrays

  void print (const Param_Test::Fixed_Array_slice *a);
  // print an array

  char *opname_;
  // operation name

  Param_Test::Fixed_Array in_;
  // in parameter

  Param_Test::Fixed_Array inout_;
  // inout parameter

  Param_Test::Fixed_Array out_;
  // out parameter

  Param_Test::Fixed_Array_var ret_;
  // return value
};

#endif /* PARAM_TEST_FIXED_ARRAY_H */
