// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    recursive_struct.h
//
// = DESCRIPTION
//    Tests a structure that contains a sequence of itself
//
// = AUTHORS
//    Aniruddha Gokhale, Jeff Parsons
//
// ============================================================================

#ifndef PARAM_TEST_RECURSIVE_STRUCT_H
#define PARAM_TEST_RECURSIVE_STRUCT_H

#include "param_testCli.h"
#include "helper.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test recursive structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Recursive_Struct
{
public:
  Test_Recursive_Struct (void);
  // ctor

  ~Test_Recursive_Struct (void);
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

  void dii_req_invoke (CORBA::Request *, CORBA::Environment &);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  Param_Test::Recursive_Struct in_;
  // in parameter

  // these need memory management
  Param_Test::Recursive_Struct_var inout_;
  // inout parameter

  Param_Test::Recursive_Struct_var out_;
  // out parameter

  Param_Test::Recursive_Struct_var ret_;
  // return value

  void deep_init (Param_Test::Recursive_Struct &rs,
                  Generator *gen,
                  CORBA::ULong level);
  // helper function for init_parameters

  CORBA::Boolean deep_check (const Param_Test::Recursive_Struct &in_struct,
                             const Param_Test::Recursive_Struct &test_strcut);
  // helper function for check_validity
};

#endif /* PARAM_TEST_RECURSIVE_STRUCT_H */
