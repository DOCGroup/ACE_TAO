
//=============================================================================
/**
 *  @file    objref.h
 *
 *   Tests object references
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_OBJREF_H
#define PARAM_TEST_OBJREF_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test objrefs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_ObjRef
{
public:
  /// ctor
  Test_ObjRef ();

  /// dtor
  ~Test_ObjRef ();

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
  char *opname_;

  /// in parameter
  Coffee_var in_;

  /// inout parameter
  Coffee_var inout_;

  /// out parameter
  Coffee_var out_;

  /// return value
  Coffee_var ret_;
};

#endif /* PARAM_TEST_OBJREF_H */
