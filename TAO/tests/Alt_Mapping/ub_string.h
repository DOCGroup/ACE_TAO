// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    unbounded_string.h
//
// = DESCRIPTION
//     Tests unbounded string
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_STRING_H
#define PARAM_TEST_UNBOUNDED_STRING_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test unbounded strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Unbounded_String
{
public:
  Test_Unbounded_String (void);
  // ctor

  ~Test_Unbounded_String (void);
  // dtor

  int run_sii_test (Alt_Mapping_ptr objref);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Alt_Mapping_ptr objref);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

private:
  char *opname_;
  // operation name

  std::string in_;
  std::string inout_;
  std::string out_;
  std::string ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_STRING_H */
