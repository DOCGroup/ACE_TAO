// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    nested_struct.h
//
// = DESCRIPTION
//     Tests nested structures
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_NESTED_STRUCT_H
#define PARAM_TEST_NESTED_STRUCT_H

#include "param_testCli.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test nested structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Nested_Struct
{
public:
  Test_Nested_Struct (void);
  // ctor

  ~Test_Nested_Struct (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr nvlist,
                CORBA::NVList_ptr retval,
                CORBA::Environment &env);
  // add args to NVList for DII

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

  Param_Test::Nested_Struct in_;
  // in parameter

  // these need memory management
  Param_Test::Nested_Struct_var inout_;
  // inout parameter

  Param_Test::Nested_Struct_var out_;
  // out parameter

  Param_Test::Nested_Struct_var ret_;
  // return value
};

#endif /* PARAM_TEST_NESTED_STRUCT_H */
