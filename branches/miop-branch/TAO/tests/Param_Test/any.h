// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    any.h
//
// = DESCRIPTION
//     Tests Anys
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_ANY_H
#define PARAM_TEST_ANY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Any
{
public:
  Test_Any (void);
  // ctor

  ~Test_Any (void);
  // dtor

  enum
  {
    ANY_SHORT = 0,
    ANY_STRING,
    ANY_OBJREF,
    ANY_ARRAY,
    ANY_SHORT_SEQ,
    ANY_BD_SHORT_SEQ,
    ANY_STRUCT,
    ANY_BIG_UNION,
    ANY_SMALL_UNION,
    ANY_LAST_TEST_ITEM
  };

  int run_sii_test (Param_Test_ptr objref
                    TAO_ENV_ARG_DECL);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref
                       TAO_ENV_ARG_DECL);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

  void dii_req_invoke (CORBA::Request * TAO_ENV_ARG_DECL_NOT_USED);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  Coffee_var cobj_;
  // Coffee object used to test Any with object.

  static size_t counter;
  // test counter.

  CORBA::Any in_;
  // in parameter

  CORBA::Any inout_;
  // inout parameter

  CORBA::Any_var out_;
  // out parameter

  CORBA::Any_var ret_;
  // return value
};

#endif /* PARAM_TEST_ANY_H */
