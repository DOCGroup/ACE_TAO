// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_short_seq.h
//
// = DESCRIPTION
//     Tests unbounded short sequence
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_SHORT_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_SHORT_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequences of shorts
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Short_Sequence
{
public:
  Test_Short_Sequence (void);
  // ctor

  ~Test_Short_Sequence (void);
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

  CORBA::ShortSeq_var in_;
  // in parameter

  CORBA::ShortSeq_var inout_;
  // inout parameter

  CORBA::ShortSeq_var out_;
  // out parameter

  CORBA::ShortSeq_var ret_;
  // return value
};

#endif /* PARAM_TEST_UNBOUNDED_SHORT_SEQUENCE_H */
