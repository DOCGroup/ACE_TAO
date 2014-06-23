
//=============================================================================
/**
 *  @file    ub_any_seq.h
 *
 *  $Id$
 *
 *   Tests unbounded Any sequences
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test sequences of Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_AnySeq
{
public:
  /// ctor
  Test_AnySeq (void);

  /// dtor
  ~Test_AnySeq (void);

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
  CORBA::Boolean compare (const CORBA::AnySeq &s1,
                          const Param_Test::StructSeq &s2);

  /// print individual sequence
  void print_sequence (const CORBA::AnySeq &s);

private:
  /// operation name
  char *opname_;

  /// in parameter
  CORBA::AnySeq_var in_;

  /// inout parameter
  CORBA::AnySeq_var inout_;

  /// out parameter
  CORBA::AnySeq_var out_;

  /// return value
  CORBA::AnySeq_var ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_ANY_SEQUENCE_H */
