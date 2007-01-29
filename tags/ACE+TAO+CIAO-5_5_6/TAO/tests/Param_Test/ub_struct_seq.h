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

#ifndef PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H
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

protected:
  CORBA::Boolean compare (const Param_Test::StructSeq &s1,
                          const Param_Test::StructSeq &s2);
  // compare

  void print_sequence (const Param_Test::StructSeq &s);
  // print individual sequence

private:
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
};



class Test_Unbounded_Struct_Sequence
{
public:
  Test_Unbounded_Struct_Sequence (void);
  // ctor

  ~Test_Unbounded_Struct_Sequence (void);
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

protected:
  CORBA::Boolean compare (const Param_Test::PathSpec &s1,
                          const Param_Test::PathSpec &s2);
  // compare

  void print_sequence (const Param_Test::PathSpec &s);
  // print individual sequence

private:
 char *opname_;
  // operation name

  Param_Test::PathSpec in_;
  // in parameter

  // these need memory management
  Param_Test::PathSpec_var inout_;
  // inout parameter

  Param_Test::PathSpec_var out_;
  // out parameter

  Param_Test::PathSpec_var ret_;
  // return value
};


#endif /* PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
