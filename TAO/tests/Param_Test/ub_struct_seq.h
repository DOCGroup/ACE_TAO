// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_struct_seq.h
//
// = DESCRIPTION
//     Tests unbounded struct sequence
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H)
#define PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequence of structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Struct_Sequence
{
public:
  Test_Struct_Sequence (void);
  // ctor

  ~Test_Struct_Sequence (void);
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

protected:
  CORBA::Boolean compare (const Param_Test::StructSeq &s1,
                          const Param_Test::StructSeq &s2);
  // compare

  void print_sequence (const Param_Test::StructSeq &s);
  // print individual sequence
private:
  // called by the 2 public versions of check_validity
  CORBA::Boolean
  Test_Struct_Sequence::check_validity_engine (const Param_Test::StructSeq &the_in,
                                               const Param_Test::StructSeq &the_inout,
                                               const Param_Test::StructSeq &the_out,
                                               const Param_Test::StructSeq &the_ret);

 char *opname_;
  // operation name

  Param_Test::StructSeq in_;
  // in parameter

  // these need memory management
  Param_Test::StructSeq_var inout_;
  // inout parameter

  Param_Test::StructSeq_var out_;
  // out parameter

  Param_Test::StructSeq_var ret_;
  // return value

  Param_Test::StructSeq dii_out_;
  // DII out parameter

  Param_Test::StructSeq dii_ret_;
  // DII return value
};

#endif /* PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
