// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    longlong.h
//
// = DESCRIPTION
//     Tests 64bit longs
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_LONGLONG_H)
#define PARAM_TEST_LONGLONG_H

#include "param_testC.h"

class Test_LongLong
{
public:
  Test_LongLong (void);
  // ctor

  ~Test_LongLong (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr nvlist,
                CORBA::NVList_ptr retval,
                CORBA::Environment &env);

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref,
                       CORBA::Environment &env);
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

  CORBA::LongLong in_;
  // in parameter

  CORBA::LongLong inout_;
  // inout parameter

  CORBA::LongLong out_;
  // out parameter

  CORBA::LongLong ret_;
  // return value
};

#endif /* PARAM_TEST_LONGLONG_H */
