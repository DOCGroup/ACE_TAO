
//=============================================================================
/**
 *  @file    ulonglong.h
 *
 *  $Id$
 *
 *   Tests 64bit unsigned longs.
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_ULONGLONG_H
#define PARAM_TEST_ULONGLONG_H

#include "param_testC.h"

class Test_ULongLong
{
public:
  /// ctor
  Test_ULongLong (void);

  /// dtor
  ~Test_ULongLong (void);

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
  CORBA::ULongLong in_;

  /// inout parameter
  CORBA::ULongLong inout_;

  /// out parameter
  CORBA::ULongLong out_;

  /// return value
  CORBA::ULongLong ret_;
};

#endif /* PARAM_TEST_ULONGLONG_H */
