// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_wstr_seq.h
//
// = DESCRIPTION
//     Tests sequences of wide strings
//
// = AUTHORS
//      Jeff Parsons
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H

#include "param_testCli.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of wstrings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_WString_Sequence
{
public:
  Test_WString_Sequence (void);
  // ctor

  ~Test_WString_Sequence (void);
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

  void dii_req_invoke (CORBA::Request *, 
                       CORBA::Environment &);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  Param_Test::WStrSeq_var in_;
  // in parameter

  Param_Test::WStrSeq_var inout_;
  // inout parameter

  Param_Test::WStrSeq_var out_;
  // out parameter

  Param_Test::WStrSeq_var ret_;
  // return value
};

#endif /* PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H */
