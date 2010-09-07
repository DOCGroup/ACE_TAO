// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_octet_seq.h
//
// = DESCRIPTION
//     Tests unbounded octet sequences
//
// = AUTHORS
//      Aniruddha Gokhale
//      Jeff Parsons
//
// ============================================================================

#ifndef ALT_MAPPING_UNBOUNDED_OCTET_SEQUENCE_H
#define ALT_MAPPING_UNBOUNDED_OCTET_SEQUENCE_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequences of octete
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Octet_Sequence
{
public:
  Test_Octet_Sequence (void);
  // ctor

  ~Test_Octet_Sequence (void);
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

  CORBA::OctetSeq in_;
  // in parameter

  CORBA::OctetSeq inout_;
  // inout parameter

  CORBA::OctetSeq out_;
  // out parameter

  CORBA::OctetSeq ret_;
  // return value
};

#endif /* ALT_MAPPING_UNBOUNDED_OCTET_SEQUENCE_H */
