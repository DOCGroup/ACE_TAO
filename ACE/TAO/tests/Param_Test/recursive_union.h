// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    recursive_union.h
//
// = DESCRIPTION
//    Tests a union that contains a sequence of itself
//
// = AUTHORS
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef PARAM_TEST_RECURSIVE_UNION_H
#define PARAM_TEST_RECURSIVE_UNION_H

#include "param_testC.h"
#include "helper.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test recursive unions
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Recursive_Union
{
public:
  Test_Recursive_Union (void);
  // ctor

  ~Test_Recursive_Union (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref
                    ACE_ENV_ARG_DECL);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref
                       ACE_ENV_ARG_DECL);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

  void dii_req_invoke (CORBA::Request *
                       ACE_ENV_ARG_DECL_NOT_USED);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  Param_Test::Recursive_Union in_;
  // in parameter

  // these need memory management
  Param_Test::Recursive_Union_var inout_;
  // inout parameter

  Param_Test::Recursive_Union_var out_;
  // out parameter

  Param_Test::Recursive_Union_var ret_;
  // return value

  void deep_init (Param_Test::Recursive_Union &u,
                  Generator *gen,
                  CORBA::ULong level);

  void deep_init_nested (Param_Test::nested_rec_union &nu,
                         Generator *gen,
                         CORBA::ULong level);
  // helper functions for init_parameters

  CORBA::Boolean deep_check (const Param_Test::Recursive_Union &in_union,
                             const Param_Test::Recursive_Union &test_union);

  CORBA::Boolean deep_check_nested (const Param_Test::nested_rec_union &in,
                                    const Param_Test::nested_rec_union &test);
  // helper functions for check_validity
};

#endif /* PARAM_TEST_RECURSIVE_UNION_H */
