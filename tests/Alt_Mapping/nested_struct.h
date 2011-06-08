// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    nested_struct.h
//
// = DESCRIPTION
//     Tests nested structures
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef ALT_MAPPING_TEST_NESTED_STRUCT_H
#define ALT_MAPPING_TEST_NESTED_STRUCT_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test nested structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Nested_Struct
{
public:
  Test_Nested_Struct (void);
  // ctor

  ~Test_Nested_Struct (void);
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

  void print_values (void);
  // print all the values

private:
  char *opname_;

  Alt_Mapping::Nested_Struct in_;

  Alt_Mapping::Nested_Struct_var inout_;

  Alt_Mapping::Nested_Struct_var out_;

  Alt_Mapping::Nested_Struct_var ret_;
};

#endif /* ALT_MAPPING_TEST_NESTED_STRUCT_H */
