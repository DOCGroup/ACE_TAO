
//=============================================================================
/**
 *  @file    fixed_array.h
 *
 *   Tests fixed size array
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_FIXED_ARRAY_H
#define PARAM_TEST_FIXED_ARRAY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed arrays
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Fixed_Array
{
public:
  /// ctor
  Test_Fixed_Array ();

  /// dtor
  ~Test_Fixed_Array ();

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
  /// compare arrays
  CORBA::Boolean compare (const Param_Test::Fixed_Array_slice *a1,
                          const Param_Test::Fixed_Array_slice *a2);

  /// print an array
  void print (const Param_Test::Fixed_Array_slice *a);

  /// operation name
  char *opname_;

  /// in parameter
  Param_Test::Fixed_Array in_;

  /// inout parameter
  Param_Test::Fixed_Array inout_;

  /// out parameter
  Param_Test::Fixed_Array out_;

  /// return value
  Param_Test::Fixed_Array_var ret_;
};

#endif /* PARAM_TEST_FIXED_ARRAY_H */
