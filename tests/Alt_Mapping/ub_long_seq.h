// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_long_seq.h
//
// = DESCRIPTION
//     Tests unbounded long sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//      Jeff Parsons
//
// ============================================================================

#ifndef ALT_MAPPING_UNBOUNDED_LONG_SEQUENCE_H
#define ALT_MAPPING_UNBOUNDED_LONG_SEQUENCE_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequences of longs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Long_Sequence
{
public:
  Test_Long_Sequence (void);
  // ctor

  ~Test_Long_Sequence (void);
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
  std::string opname_;
  // operation name

  CORBA::LongSeq in_;
  // in parameter

  CORBA::LongSeq inout_;
  // inout parameter

  CORBA::LongSeq out_;
  // out parameter

  CORBA::LongSeq ret_;
  // return value
};

#endif /* ALT_MAPPING_UNBOUNDED_LONG_SEQUENCE_H */
