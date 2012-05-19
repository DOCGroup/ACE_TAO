
//=============================================================================
/**
 *  @file    ub_struct_seq.h
 *
 *  $Id$
 *
 *   Tests unbounded struct sequence
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H
#define PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequence of structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Struct_Sequence
{
public:
  /// ctor
  Test_Struct_Sequence (void);

  /// dtor
  ~Test_Struct_Sequence (void);

  /// run the SII test
  int run_sii_test (Param_Test_ptr objref);

  /// return operation name
  const char *opname (void) const;

  /// set values for parameters
  int init_parameters (Param_Test_ptr objref);

  /// reset values for CORBA
  int reset_parameters (void);

  /// check if results are valid
  CORBA::Boolean check_validity (void);

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values (void);

  /// invoke DII request with appropriate exception handling.
  void dii_req_invoke (CORBA::Request *);

protected:
  /// compare
  CORBA::Boolean compare (const Param_Test::StructSeq &s1,
                          const Param_Test::StructSeq &s2);

  /// print individual sequence
  void print_sequence (const Param_Test::StructSeq &s);

private:
  /// operation name
 char *opname_;

  /// in parameter
  Param_Test::StructSeq in_;

  // these need memory management
  /// inout parameter
  Param_Test::StructSeq_var inout_;

  /// out parameter
  Param_Test::StructSeq_var out_;

  /// return value
  Param_Test::StructSeq_var ret_;
};



class Test_Unbounded_Struct_Sequence
{
public:
  /// ctor
  Test_Unbounded_Struct_Sequence (void);

  /// dtor
  ~Test_Unbounded_Struct_Sequence (void);

  /// run the SII test
  int run_sii_test (Param_Test_ptr objref);

  /// return operation name
  const char *opname (void) const;

  /// set values for parameters
  int init_parameters (Param_Test_ptr objref);

  /// reset values for CORBA
  int reset_parameters (void);

  /// check if results are valid
  CORBA::Boolean check_validity (void);

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values (void);

  /// invoke DII request with appropriate exception handling.
  void dii_req_invoke (CORBA::Request *);

protected:
  /// compare
  CORBA::Boolean compare (const Param_Test::PathSpec &s1,
                          const Param_Test::PathSpec &s2);

  /// print individual sequence
  void print_sequence (const Param_Test::PathSpec &s);

private:
  /// operation name
 char *opname_;

  /// in parameter
  Param_Test::PathSpec in_;

  // these need memory management
  /// inout parameter
  Param_Test::PathSpec_var inout_;

  /// out parameter
  Param_Test::PathSpec_var out_;

  /// return value
  Param_Test::PathSpec_var ret_;
};


#endif /* PARAM_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
