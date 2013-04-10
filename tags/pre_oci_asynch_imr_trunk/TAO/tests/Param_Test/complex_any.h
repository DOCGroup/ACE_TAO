
//=============================================================================
/**
 *  @file     complex_any.h
 *
 *  $Id$
 *
 *   Tests complex Anys
 *
 *
 *  @author  Aniruddha Gokhale
 *  @author Jeff Parsons
 *  @author Frank Buschmann
 */
//=============================================================================


#ifndef PARAM_TEST_COMPLEX_ANY_H
#define PARAM_TEST_COMPLEX_ANY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test complex Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Complex_Any
{
public:
  /// ctor
  Test_Complex_Any (void);

  /// dtor
  ~Test_Complex_Any (void);

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

  /// test counter.
  static size_t counter;

  /// in parameter
  CORBA::Any in_;

  /// inout parameter
  CORBA::Any inout_;

  /// out parameter
  CORBA::Any_var out_;

  /// return value
  CORBA::Any_var ret_;

  /// Compares the sequence extracted from in_ to one of the others.
  CORBA::Boolean comp_engine (const CORBA::AnySeq *in_seq,
                              const CORBA::AnySeq *comp_seq);
};

#endif /* PARAM_TEST_COMPLEX_ANY_H */
