// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    big_union.h
//
// = DESCRIPTION
//     Tests Anys
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_BIG_UNION_H
#define PARAM_TEST_BIG_UNION_H

#include "param_testCli.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Big_Union
{
public:
  Test_Big_Union (void);
  // ctor

  ~Test_Big_Union (void);
  // dtor

  enum {
    BIG_UNION_N_BRANCHES = 12
  };

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

  Coffee_var cobj_;
  // Coffee object used to test Big_Union with object.

  static size_t counter;
  // test counter.

  Param_Test::Big_Union in_;
  // in parameter

  Param_Test::Big_Union inout_;
  // inout parameter

  Param_Test::Big_Union_var out_;
  // out parameter

  Param_Test::Big_Union_var ret_;
  // return value
};

#endif /* PARAM_TEST_BIG_UNION_H */
