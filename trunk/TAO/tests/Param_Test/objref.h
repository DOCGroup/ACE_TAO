// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    objref.h
//
// = DESCRIPTION
//     Tests object references 
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_OBJREF_H)
#define PARAM_TEST_OBJREF_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test objrefs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_ObjRef
{
public:
  Test_ObjRef (void);
  // ctor

  ~Test_ObjRef (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr nvlist,
                CORBA::NVList_ptr retval,
                CORBA::Environment &env);
  // add args to NVList for DII

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

  Coffee_var in_;
  // in parameter

  Coffee_var inout_;
  // inout parameter

  Coffee_var out_;
  // out parameter

  Coffee_var ret_;
  // return value
};

#endif /* PARAM_TEST_OBJREF_H */
