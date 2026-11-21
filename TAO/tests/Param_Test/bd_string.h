/**
 *  @file    bd_string.h
 *
 *   Tests bounded string
 *
 *  @author   Carlos O'Ryan
 */

#ifndef PARAM_TEST_BOUNDED_STRING_H
#define PARAM_TEST_BOUNDED_STRING_H

#include "param_testC.h"

/// tests bounded strings
class Test_Bounded_String
{
public:
  Test_Bounded_String ();

  ~Test_Bounded_String () = default;

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
  char const *opname_;

  /// in parameter
  CORBA::String_var in_;

  /// inout parameter
  CORBA::String_var inout_;

  /// out parameter
  CORBA::String_var out_;

  /// return value
  CORBA::String_var ret_;
};

#endif /* PARAM_TEST_BOUNDED_STRING_H */
