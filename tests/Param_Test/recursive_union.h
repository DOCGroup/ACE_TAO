
//=============================================================================
/**
 *  @file    recursive_union.h
 *
 *  $Id$
 *
 *  Tests a union that contains a sequence of itself
 *
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef PARAM_TEST_RECURSIVE_UNION_H
#define PARAM_TEST_RECURSIVE_UNION_H

#include "param_testC.h"
#include "helper.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test recursive unions
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Recursive_Union
{
public:
  /// ctor
  Test_Recursive_Union (void);

  /// dtor
  ~Test_Recursive_Union (void);

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
  Param_Test::Recursive_Union in_;

  // these need memory management
  /// inout parameter
  Param_Test::Recursive_Union_var inout_;

  /// out parameter
  Param_Test::Recursive_Union_var out_;

  /// return value
  Param_Test::Recursive_Union_var ret_;

  void deep_init (Param_Test::Recursive_Union &u,
                  Generator *gen,
                  CORBA::ULong level);

  /// helper functions for init_parameters
  void deep_init_nested (Param_Test::nested_rec_union &nu,
                         Generator *gen,
                         CORBA::ULong level);

  CORBA::Boolean deep_check (const Param_Test::Recursive_Union &in_union,
                             const Param_Test::Recursive_Union &test_union);

  /// helper functions for check_validity
  CORBA::Boolean deep_check_nested (const Param_Test::nested_rec_union &in,
                                    const Param_Test::nested_rec_union &test);
};

#endif /* PARAM_TEST_RECURSIVE_UNION_H */
