// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    bd_short_seq.h
//
// = DESCRIPTION
//     Tests bounded short sequence
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_BOUNDED_SHORT_SEQUENCE_H)
#define PARAM_TEST_BOUNDED_SHORT_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test bounded sequences of shorts
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Bounded_Short_Sequence
{
public:
  Test_Bounded_Short_Sequence (void);
  // ctor

  ~Test_Bounded_Short_Sequence (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr &nvlist,
                CORBA::NVList_ptr &retval,
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

private:
  char *opname_;
  // operation name

  Param_Test::Bounded_Short_Seq_var in_;
  // in parameter

  Param_Test::Bounded_Short_Seq_var inout_;
  // inout parameter

  Param_Test::Bounded_Short_Seq_var out_;
  // out parameter

  Param_Test::Bounded_Short_Seq_var ret_;
  // return value
};

#endif /* PARAM_TEST_BOUNDED_SHORT_SEQUENCE_H */
