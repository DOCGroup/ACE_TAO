
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
#include <string>

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test unbounded strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Unbounded_String
{
public:
  /// ctor
  Test_Unbounded_String () = default;

  /// dtor
  ~Test_Unbounded_String () = default;
  Test_Unbounded_String (Test_Unbounded_String const &) = delete;
  Test_Unbounded_String (Test_Unbounded_String &&) = delete;
  Test_Unbounded_String &operator= (Test_Unbounded_String const &) = delete;
  Test_Unbounded_String &operator= (Test_Unbounded_String &&) = delete;

  /// run the SII test
  int run_sii_test (Alt_Mapping_ptr objref);

  /// return operation name
  char const *opname () const;

  /// set values for parameters
  int init_parameters (Alt_Mapping_ptr objref);

  /// reset values for CORBA
  int reset_parameters ();

  /// check if results are valid
  CORBA::Boolean check_validity () const;

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req) const;

  /// print all the values
  void print_values () const;

private:
  /// operation name
  std::string opname_ {"test_unbounded_string"};

  std::string in_;
  std::string inout_;
  std::string out_;
  std::string ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_STRING_H */
