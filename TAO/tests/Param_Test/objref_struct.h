
//=============================================================================
/**
 *  @file    objref_struct.h
 *
 *  $Id$
 *
 *   Tests variable sized structures
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_OBJREF_STRUCT_H
#define PARAM_TEST_OBJREF_STRUCT_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test variable sized structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Objref_Struct
{
public:
  /// ctor
  Test_Objref_Struct (void);

  /// dtor
  ~Test_Objref_Struct (void);

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
  Param_Test::Objref_Struct in_;

  // these need memory management
  /// inout parameter
  Param_Test::Objref_Struct_var inout_;

  /// out parameter
  Param_Test::Objref_Struct_var out_;

  /// return value
  Param_Test::Objref_Struct_var ret_;
};

#endif /* PARAM_TEST_OBJREF_STRUCT_H */
