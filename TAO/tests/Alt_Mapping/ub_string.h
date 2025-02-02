
//=============================================================================
/**
 *  @file    ub_string.h
 *
 *   Tests unbounded string
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_STRING_H
#define PARAM_TEST_UNBOUNDED_STRING_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test unbounded strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Unbounded_String
{
public:
  /// ctor
  Test_Unbounded_String ();

  /// dtor
  ~Test_Unbounded_String ();

  /// run the SII test
  int run_sii_test (Alt_Mapping_ptr objref);

  /// return operation name
  const char *opname () const;

  /// set values for parameters
  int init_parameters (Alt_Mapping_ptr objref);

  /// reset values for CORBA
  int reset_parameters ();

  /// check if results are valid
  CORBA::Boolean check_validity ();

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values ();

private:
  /// operation name
  char *opname_;

  std::string in_;
  std::string inout_;
  std::string out_;
  std::string ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_STRING_H */
