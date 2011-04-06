
//=============================================================================
/**
 *  @file    recursive_struct.h
 *
 *  $Id$
 *
 *  Tests a structure that contains a sequence of itself
 *
 *
 *  @author Aniruddha Gokhale
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef PARAM_TEST_RECURSIVE_STRUCT_H
#define PARAM_TEST_RECURSIVE_STRUCT_H

#include "param_testC.h"
#include "helper.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test recursive structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Recursive_Struct
{
public:
  /// ctor
  Test_Recursive_Struct (void);

  /// dtor
  ~Test_Recursive_Struct (void);

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
  Param_Test::Recursive_Struct in_;

  // these need memory management
  /// inout parameter
  Param_Test::Recursive_Struct_var inout_;

  /// out parameter
  Param_Test::Recursive_Struct_var out_;

  /// return value
  Param_Test::Recursive_Struct_var ret_;

  /// helper function for init_parameters
  void deep_init (Param_Test::Recursive_Struct &rs,
                  Generator *gen,
                  CORBA::ULong level);

  /// helper function for check_validity
  CORBA::Boolean deep_check (const Param_Test::Recursive_Struct &in_struct,
                             const Param_Test::Recursive_Struct &test_struct);
};

#endif /* PARAM_TEST_RECURSIVE_STRUCT_H */
