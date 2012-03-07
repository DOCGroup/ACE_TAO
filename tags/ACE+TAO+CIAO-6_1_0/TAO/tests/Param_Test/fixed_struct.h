
//=============================================================================
/**
 *  @file    fixed_struct.h
 *
 *  $Id$
 *
 *   Tests fixed size structs
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_FIXED_STRUCT_H
#define PARAM_TEST_FIXED_STRUCT_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Fixed_Struct
{
public:
  /// ctor
  Test_Fixed_Struct (void);

  /// dtor
  ~Test_Fixed_Struct (void);

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
  Param_Test::Fixed_Struct in_;

  /// inout parameter
  Param_Test::Fixed_Struct inout_;

  /// out parameter
  Param_Test::Fixed_Struct out_;

  /// return value
  Param_Test::Fixed_Struct ret_;
};

#endif /* PARAM_TEST_FIXED_STRUCT_H */
