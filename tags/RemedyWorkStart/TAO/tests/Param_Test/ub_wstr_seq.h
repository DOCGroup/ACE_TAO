
//=============================================================================
/**
 *  @file    ub_wstr_seq.h
 *
 *  $Id$
 *
 *   Tests sequences of wide strings
 *
 *
 *  @author   Jeff Parsons
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of wstrings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_WString_Sequence
{
public:
  /// ctor
  Test_WString_Sequence (void);

  /// dtor
  ~Test_WString_Sequence (void);

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
  CORBA::WStringSeq_var in_;

  /// inout parameter
  CORBA::WStringSeq_var inout_;

  /// out parameter
  CORBA::WStringSeq_var out_;

  /// return value
  CORBA::WStringSeq_var ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_WSTRING_SEQUENCE_H */
