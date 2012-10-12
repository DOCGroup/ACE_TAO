
//=============================================================================
/**
 *  @file    ub_long_seq.h
 *
 *  $Id$
 *
 *   Tests unbounded long sequences
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequences of longs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Long_Sequence
{
public:
  /// ctor
  Test_Long_Sequence (void);

  /// dtor
  ~Test_Long_Sequence (void);

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

private:
  /// operation name
  char *opname_;

  /// in parameter
  CORBA::LongSeq_var in_;

  /// inout parameter
  CORBA::LongSeq_var inout_;

  /// out parameter
  CORBA::LongSeq_var out_;

  /// return value
  CORBA::LongSeq_var ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_LONG_SEQUENCE_H */
