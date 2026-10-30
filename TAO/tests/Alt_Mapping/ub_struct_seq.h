
//=============================================================================
/**
 *  @file    ub_struct_seq.h
 *
 *   Tests unbounded struct sequence
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H
#define ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H

#include "alt_mappingC.h"
#include <string>

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test unbounded sequence of structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Unbounded_Struct_Sequence
{
public:
  /// ctor
  Test_Unbounded_Struct_Sequence () = default;

  /// dtor
  ~Test_Unbounded_Struct_Sequence () = default;
  Test_Unbounded_Struct_Sequence (Test_Unbounded_Struct_Sequence const &) = delete;
  Test_Unbounded_Struct_Sequence (Test_Unbounded_Struct_Sequence &&) = delete;
  Test_Unbounded_Struct_Sequence &operator= (Test_Unbounded_Struct_Sequence const &) = delete;
  Test_Unbounded_Struct_Sequence &operator= (Test_Unbounded_Struct_Sequence &&) = delete;

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

  /// print all the values
  void print_values () const;

protected:
  /// compare
  CORBA::Boolean compare (Alt_Mapping::StructSeq const &s1,
                          Alt_Mapping::StructSeq const &s2) const;

  /// print individual sequence
  void print_sequence (Alt_Mapping::StructSeq const &s) const;

private:
  std::string opname_ {"test_struct_sequence"};

  Alt_Mapping::StructSeq in_;

  Alt_Mapping::StructSeq inout_;

  Alt_Mapping::StructSeq out_;

  Alt_Mapping::StructSeq ret_;
};

#endif /* ALT_MAPPING_TEST_UNBOUNED_STRUCT_SEQUENCE_H */
