
//=============================================================================
/**
 *  @file    any.h
 *
 *  $Id$
 *
 *   Tests Anys
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_ANY_H
#define PARAM_TEST_ANY_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test Anys
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Any
{
public:
  /// ctor
  Test_Any (void);

  /// dtor
  ~Test_Any (void);

  enum
  {
    ANY_SHORT = 0,
    ANY_STRING,
    ANY_OBJREF,
    ANY_ARRAY,
    ANY_SHORT_SEQ,
    ANY_BD_SHORT_SEQ,
    ANY_STRUCT,
    ANY_BIG_UNION,
    ANY_SMALL_UNION,
    ANY_LAST_TEST_ITEM
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

  /// Coffee object used to test Any with object.
  Coffee_var cobj_;

  /// test counter.
  static size_t counter;

  /// in parameter
  CORBA::Any in_;

  /// inout parameter
  CORBA::Any inout_;

  /// out parameter
  CORBA::Any_var out_;

  /// return value
  CORBA::Any_var ret_;
};

#endif /* PARAM_TEST_ANY_H */
