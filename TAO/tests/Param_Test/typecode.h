// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    typecode.h
//
// = DESCRIPTION
//     Tests typecodes
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_TYPECODE_H)
#define PARAM_TEST_TYPECODE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test TypeCodes
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_TypeCode
{
public:
  Test_TypeCode (void);
  // ctor

  ~Test_TypeCode (void);
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
  CORBA::TypeCode_var tc_holder_;
  // place for selected typecode to stay during time
  // between init_parameters () and reset_parameters ().
  
  // called by the 2 public versions of check_validity
  CORBA::Boolean
  Test_TypeCode::check_validity_engine (CORBA::TypeCode *the_in,
                                        CORBA::TypeCode *the_inout,
                                        CORBA::TypeCode *the_out,
                                        CORBA::TypeCode *the_ret);

  char *opname_;
  // operation name

  CORBA::TypeCode_var in_;
  // in parameter

  CORBA::TypeCode_var inout_;
  // inout parameter

  CORBA::TypeCode_var out_;
  // out parameter

  CORBA::TypeCode_var ret_;
  // return value

  CORBA::TypeCode dii_out_;
  // DII out parameter

  CORBA::TypeCode dii_ret_;
  // DII return value
};

#endif /* PARAM_TEST_TYPECODE_H */
