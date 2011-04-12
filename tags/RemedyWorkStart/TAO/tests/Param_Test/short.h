
//=============================================================================
/**
 *  @file    short.h
 *
 *  $Id$
 *
 *   Tests shorts
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_SHORT_H
#define PARAM_TEST_SHORT_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test shorts
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Short
{
public:
  /// ctor
  Test_Short (void);

  /// dtor
  ~Test_Short (void);

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
  CORBA::Short in_;

  /// inout parameter
  CORBA::Short inout_;

  /// out parameter
  CORBA::Short out_;

  /// return value
  CORBA::Short ret_;
};

#endif /* PARAM_TEST_SHORT_H */
