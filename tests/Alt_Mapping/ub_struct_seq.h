// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_struct_seq.h
//
// = DESCRIPTION
//     Tests unbounded struct sequence
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H
#define ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test unbounded sequence of structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Unbounded_Struct_Sequence
{
public:
  Test_Unbounded_Struct_Sequence (void);
  // ctor

  ~Test_Unbounded_Struct_Sequence (void);
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

protected:
  CORBA::Boolean compare (const Alt_Mapping::StructSeq &s1,
                          const Alt_Mapping::StructSeq &s2);
  // compare

  void print_sequence (const Alt_Mapping::StructSeq &s);
  // print individual sequence

private:
 char *opname_;

  Alt_Mapping::StructSeq in_;

  Alt_Mapping::StructSeq inout_;

  Alt_Mapping::StructSeq out_;

  Alt_Mapping::StructSeq ret_;
};

#endif /* ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
