
//=============================================================================
/**
 *  @file    typecode.h
 *
 *  $Id$
 *
 *   Tests typecodes
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_TYPECODE_H
#define PARAM_TEST_TYPECODE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test TypeCodes
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_TypeCode
{
public:
  /// ctor
  Test_TypeCode (void);

  /// dtor
  ~Test_TypeCode (void);

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
  /// place for selected typecode to stay during time
  /// between init_parameters () and reset_parameters ().
  CORBA::TypeCode_var tc_holder_;

  /// operation name
  char *opname_;

  /// in parameter
  CORBA::TypeCode_var in_;

  /// inout parameter
  CORBA::TypeCode_var inout_;

  /// out parameter
  CORBA::TypeCode_var out_;

  /// return value
  CORBA::TypeCode_var ret_;
};

#endif /* PARAM_TEST_TYPECODE_H */
