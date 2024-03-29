
//=============================================================================
/**
 *  @file    ub_wstring.h
 *
 *   Tests unbounded wide string
 *
 *  @author   Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_WSTRING_H
#define PARAM_TEST_UNBOUNDED_WSTRING_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test unbounded wide strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Unbounded_WString
{
public:
  /// ctor
  Test_Unbounded_WString ();

  /// dtor
  ~Test_Unbounded_WString ();

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
  CORBA::WChar *in_;

  /// inout parameter
  CORBA::WChar *inout_;

  /// out parameter
  CORBA::WChar *out_;

  /// return value
  CORBA::WChar *ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_WSTRING_H */
