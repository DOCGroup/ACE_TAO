// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_long_seq.h
//
// = DESCRIPTION
//     Tests unbounded long sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequences of longs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Long_Sequence
{
public:
  Test_Long_Sequence (void);
  // ctor

  ~Test_Long_Sequence (void);
  // dtor

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

  Param_Test::Long_Seq_var in_;
  // in parameter

  Param_Test::Long_Seq_var inout_;
  // inout parameter

  Param_Test::Long_Seq_var out_;
  // out parameter

  Param_Test::Long_Seq_var ret_;
  // return value
};

#endif /* PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H */
