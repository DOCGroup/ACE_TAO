
//=============================================================================
/**
 *  @file    ub_objref_seq.h
 *
 *  $Id$
 *
 *   Tests unbounded sequences of object references
 *
 *
 *  @author   Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_UNBOUNDED_OBJREF_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_OBJREF_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test sequence of object references
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_ObjRef_Sequence
{
public:
  /// ctor
  Test_ObjRef_Sequence (void);

  /// dtor
  ~Test_ObjRef_Sequence (void);

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
  CORBA::Boolean compare (const Param_Test::Coffee_Mix &s1,
                          const Param_Test::Coffee_Mix &s2);

  /// print individual sequence
  void print_sequence (const Param_Test::Coffee_Mix &s);
private:
  /// operation name
  char *opname_;

  /// in parameter
  Param_Test::Coffee_Mix in_;

  // these need memory management
  /// inout parameter
  Param_Test::Coffee_Mix_var inout_;

  /// out parameter
  Param_Test::Coffee_Mix_var out_;

  /// return value
  Param_Test::Coffee_Mix_var ret_;
};

#endif /* PARAM_TEST_UNBOUNDED_OBJREF_SEQUENCE_H */
