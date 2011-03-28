
//=============================================================================
/**
 *  @file    multdim_array.h
 *
 *  $Id$
 *
 *   Tests multi dimensional array
 *
 *
 *  @author Bala
 */
//=============================================================================


#ifndef PARAM_TEST_MULTDIM_ARRAY_H
#define PARAM_TEST_MULTDIM_ARRAY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed size multidimensional arrays
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Multdim_Array
{
public:
  /// ctor
  Test_Multdim_Array (void);

  /// dtor
  ~Test_Multdim_Array (void);

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
  /// compare arrays
  CORBA::Boolean compare (const Param_Test::Multdim_Array_slice *a1,
                          const Param_Test::Multdim_Array_slice *a2);

  /// print an array
  void print (const Param_Test::Multdim_Array_slice *a);

  /// operation name
  char *opname_;

  /// in parameter
  Param_Test::Multdim_Array_var in_;

  /// inout parameter
  Param_Test::Multdim_Array_var inout_;

  /// out parameter
  Param_Test::Multdim_Array_var out_;

  /// return value
  Param_Test::Multdim_Array_var ret_;
};

#endif /* PARAM_TEST_MULTDIM_ARRAY_H */
