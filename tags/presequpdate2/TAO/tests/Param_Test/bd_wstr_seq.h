// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_wstr_seq.h
//
// = DESCRIPTION
//     Tests bounded sequences of wide strings
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#ifndef PARAM_TEST_BOUNDED_WSTRING_SEQUENCE_H
#define PARAM_TEST_BOUNDED_WSTRING_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of wstrings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Bounded_WString_Sequence
{
public:
  Test_Bounded_WString_Sequence (void);
  // ctor

  ~Test_Bounded_WString_Sequence (void);
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

  Param_Test::Bounded_WStrSeq_var in_;
  // in parameter

  Param_Test::Bounded_WStrSeq_var inout_;
  // inout parameter

  Param_Test::Bounded_WStrSeq_var out_;
  // out parameter

  Param_Test::Bounded_WStrSeq_var ret_;
  // return value
};

#endif /* PARAM_TEST_BOUNDED_WSTRING_SEQUENCE_H */
