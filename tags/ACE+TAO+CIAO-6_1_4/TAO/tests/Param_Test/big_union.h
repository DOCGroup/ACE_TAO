
//=============================================================================
/**
 *  @file    big_union.h
 *
 *  $Id$
 *
 *   Tests Anys
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_BIG_UNION_H
#define PARAM_TEST_BIG_UNION_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Big_Union
{
public:
  /// ctor
  Test_Big_Union (void);

  /// dtor
  ~Test_Big_Union (void);

  enum {
    BIG_UNION_N_BRANCHES = 12
  };

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

  /// Coffee object used to test Big_Union with object.
  Coffee_var cobj_;

  /// test counter.
  static size_t counter;

  /// in parameter
  Param_Test::Big_Union in_;

  /// inout parameter
  Param_Test::Big_Union inout_;

  /// out parameter
  Param_Test::Big_Union_var out_;

  /// return value
  Param_Test::Big_Union_var ret_;
};

#endif /* PARAM_TEST_BIG_UNION_H */
