// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_any_seq.h
//
// = DESCRIPTION
//     Tests unbounded Any sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H

#include "param_testCli.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test sequences of Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_AnySeq
{
public:
  Test_AnySeq (void);
  // ctor

  ~Test_AnySeq (void);
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

protected:
  CORBA::Boolean compare (const Param_Test::AnySeq &s1,
                          const Param_Test::StructSeq &s2);
  // compare

  void print_sequence (const Param_Test::AnySeq &s);
  // print individual sequence

private:
  char *opname_;
  // operation name

  Param_Test::AnySeq_var in_;
  // in parameter

  Param_Test::AnySeq_var inout_;
  // inout parameter

  Param_Test::AnySeq_var out_;
  // out parameter

  Param_Test::AnySeq_var ret_;
  // return value
};

#endif /* PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H */
