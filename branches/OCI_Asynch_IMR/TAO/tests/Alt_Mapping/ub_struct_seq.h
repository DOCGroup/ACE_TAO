
//=============================================================================
/**
 *  @file    ub_struct_seq.h
 *
 *  $Id$
 *
 *   Tests unbounded struct sequence
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H
#define ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test unbounded sequence of structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Unbounded_Struct_Sequence
{
public:
  /// ctor
  Test_Unbounded_Struct_Sequence (void);

  /// dtor
  ~Test_Unbounded_Struct_Sequence (void);

  /// run the SII test
  int run_sii_test (Alt_Mapping_ptr objref);

  /// return operation name
  const char *opname (void) const;

  /// set values for parameters
  int init_parameters (Alt_Mapping_ptr objref);

  /// reset values for CORBA
  int reset_parameters (void);

  /// check if results are valid
  CORBA::Boolean check_validity (void);

  /// print all the values
  void print_values (void);

protected:
  /// compare
  CORBA::Boolean compare (const Alt_Mapping::StructSeq &s1,
                          const Alt_Mapping::StructSeq &s2);

  /// print individual sequence
  void print_sequence (const Alt_Mapping::StructSeq &s);

private:
 char *opname_;

  Alt_Mapping::StructSeq in_;

  Alt_Mapping::StructSeq inout_;

  Alt_Mapping::StructSeq out_;

  Alt_Mapping::StructSeq ret_;
};

#endif /* ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
