
//=============================================================================
/**
 *  @file    bd_str_seq.h
 *
 *   Tests bounded sequences of strings
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_BOUNDED_STRING_SEQUENCE_H
#define PARAM_TEST_BOUNDED_STRING_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of strings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Bounded_String_Sequence
{
public:
  /// ctor
  Test_Bounded_String_Sequence ();

  /// dtor
  ~Test_Bounded_String_Sequence ();

  /// run the SII test
  int run_sii_test (Param_Test_ptr objref);

  /// return operation name
  const char *opname () const;

  /// set values for parameters
  int init_parameters (Param_Test_ptr objref);

  /// reset values for CORBA
  int reset_parameters ();

  /// check if results are valid
  CORBA::Boolean check_validity ();

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values ();

  /// invoke DII request with appropriate exception handling.
  void dii_req_invoke (CORBA::Request *);

private:
  /// operation name
  char *opname_;

  /// in parameter
  Param_Test::Bounded_StrSeq_var in_;

  /// inout parameter
  Param_Test::Bounded_StrSeq_var inout_;

  /// out parameter
  Param_Test::Bounded_StrSeq_var out_;

  /// return value
  Param_Test::Bounded_StrSeq_var ret_;
};

#endif /* PARAM_TEST_BOUNDED_STRING_SEQUENCE_H */
