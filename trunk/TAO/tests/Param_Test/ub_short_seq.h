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

#if !defined (PARAM_TEST_UNBOUNDED_SHORT_SEQUENCE_H)
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

private:
  // called by the 2 public versions of check_validity
  CORBA::Boolean
  Test_Short_Sequence::check_validity_engine (const Param_Test::Short_Seq &the_in,
                                              const Param_Test::Short_Seq &the_inout,
                                              const Param_Test::Short_Seq &the_out,
                                              const Param_Test::Short_Seq &the_ret);

  char *opname_;
  // operation name

  Param_Test::Short_Seq_var in_;
  // in parameter

  Param_Test::Short_Seq_var inout_;
  // inout parameter

  Param_Test::Short_Seq_var out_;
  // out parameter

  Param_Test::Short_Seq_var ret_;
  // return value

  Param_Test::Short_Seq dii_out_;
  // DII out parameter

  Param_Test::Short_Seq dii_ret_;
  // DII return value
};

#endif /* PARAM_TEST_UNBOUNDED_SHORT_SEQUENCE_H */
