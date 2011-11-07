
//=============================================================================
/**
 *  @file    bd_wstring.h
 *
 *  $Id$
 *
 *   Tests bounded wide string
 *
 *
 *  @author   Jeff Parsons
 */
//=============================================================================


#ifndef PARAM_TEST_BOUNDED_WSTRING_H
#define PARAM_TEST_BOUNDED_WSTRING_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        tests bounded wide strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Bounded_WString
{
public:
  /// ctor
  Test_Bounded_WString (void);

  /// dtor
  ~Test_Bounded_WString (void);

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
  CORBA::WChar *in_;

  /// inout parameter
  CORBA::WChar *inout_;

  /// out parameter
  CORBA::WChar *out_;

  /// return value
  CORBA::WChar *ret_;
};

#endif /* PARAM_TEST_BOUNDED_WSTRING_H */
