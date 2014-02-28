
//=============================================================================
/**
 *  @file    bd_array_seq.h
 *
 *  $Id$
 *
 *   Tests bounded array sequence
 *
 *
 *  @author   Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef PARAM_TEST_BOUNDED_ARRAY_SEQUENCE_H
#define PARAM_TEST_BOUNDED_ARRAY_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test bounded sequence of arrays
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_Bounded_Array_Sequence
{
public:
  /// ctor
  Test_Bounded_Array_Sequence (void);

  /// dtor
  ~Test_Bounded_Array_Sequence (void);

  /// run the SII test
  int run_sii_test (Param_Test_ptr objref);

  /// return operation name
  const char *opname (void) const;

  /// set values for parameters
  int init_parameters (Param_Test_ptr objref);

  /// reset values for CORBA
  int reset_parameters (void);

  /// check if results are valid
  CORBA::Boolean check_validity (void);

  /// check if results are valid. This is used for DII results
  CORBA::Boolean check_validity (CORBA::Request_ptr req);

  /// print all the values
  void print_values (void);

  /// invoke DII request with appropriate exception handling.
  void dii_req_invoke (CORBA::Request *);

protected:
  /// compare
  CORBA::Boolean compare (const Param_Test::Bounded_ArraySeq &s1,
                          const Param_Test::Bounded_ArraySeq &s2);

  /// print individual sequence
  void print_sequence (const Param_Test::Bounded_ArraySeq &s);

private:
  /// operation name
  char *opname_;

  /// in parameter
  Param_Test::Bounded_ArraySeq in_;

  // these need memory management
  /// inout parameter
  Param_Test::Bounded_ArraySeq_var inout_;

  /// out parameter
  Param_Test::Bounded_ArraySeq_var out_;

  /// return value
  Param_Test::Bounded_ArraySeq_var ret_;
};

#endif /* PARAM_TEST_BOUNDED_ARRAY_SEQUENCE_H */
