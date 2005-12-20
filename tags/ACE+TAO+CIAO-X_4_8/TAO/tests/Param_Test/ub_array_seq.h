// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_array_seq.h
//
// = DESCRIPTION
//     Tests unbounded array sequence
//
// = AUTHORS
//      Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNED_ARRAY_SEQUENCE_H
#define PARAM_TEST_UNBOUNED_ARRAY_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequence of arrays
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Array_Sequence
{
public:
  Test_Array_Sequence (void);
  // ctor

  ~Test_Array_Sequence (void);
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

protected:
  CORBA::Boolean compare (const Param_Test::ArraySeq &s1,
                          const Param_Test::ArraySeq &s2);
  // compare

  void print_sequence (const Param_Test::ArraySeq &s);
  // print individual sequence

private:
 char *opname_;
  // operation name

  Param_Test::ArraySeq in_;
  // in parameter

  // these need memory management
  Param_Test::ArraySeq_var inout_;
  // inout parameter

  Param_Test::ArraySeq_var out_;
  // out parameter

  Param_Test::ArraySeq_var ret_;
  // return value
};


#endif /* PARAM_TEST_UNBOUNED_ARRAY_SEQUENCE_H */
