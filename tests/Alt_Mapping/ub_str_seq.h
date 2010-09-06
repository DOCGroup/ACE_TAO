// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_str_seq.h
//
// = DESCRIPTION
//     Tests sequences of strings
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H

#include "alt_mappingC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of strings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_String_Sequence
{
public:
  Test_String_Sequence (void);

  ~Test_String_Sequence (void);

  int run_sii_test (Alt_Mapping_ptr objref);

  const char *opname (void) const;

  int init_parameters (Alt_Mapping_ptr objref);

  int reset_parameters (void);

  CORBA::Boolean check_validity (void);

  void print_values (void);

private:
  char *opname_;

  CORBA::StringSeq in_;

  CORBA::StringSeq inout_;

  CORBA::StringSeq out_;

  CORBA::StringSeq ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H */
