// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    tests.h
//
// = DESCRIPTION
//    All the test objects defined here
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#if !defined (TESTS_H)
#define TESTS_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test shorts
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Short
{
public:
  Test_Short (void);
  // ctor

  ~Test_Short (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr &nvlist,
                CORBA::NVList_ptr &retval,
                CORBA::Environment &env);
  // add args to NVList for DII

  const char *opname (void) const;
  // return operation name

  void init_parameters (void);
  // set values for parameters

  void reset_parameters (void);
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

  CORBA::Short in_;
  // in parameter

  CORBA::Short inout_;
  // inout parameter

  CORBA::Short out_;
  // out parameter

  CORBA::Short ret_;
  // return value
};

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test unbounded strings
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Unbounded_String
{
public:
  Test_Unbounded_String (void);
  // ctor

  ~Test_Unbounded_String (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr &nvlist,
                CORBA::NVList_ptr &retval,
                CORBA::Environment &env);
  // add args to NVList for DII

  const char *opname (void) const;
  // return operation name

  void init_parameters (void);
  // set values for parameters

  void reset_parameters (void);
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

  char *in_;
  // in parameter

  char *inout_;
  // inout parameter

  char *out_;
  // out parameter

  char *ret_;
  // return value
};


// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//                        test fixed structs
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
class Test_Fixed_Struct
{
public:
  Test_Fixed_Struct (void);
  // ctor

  ~Test_Fixed_Struct (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr &nvlist,
                CORBA::NVList_ptr &retval,
                CORBA::Environment &env);
  // add args to NVList for DII

  const char *opname (void) const;
  // return operation name

  void init_parameters (void);
  // set values for parameters

  void reset_parameters (void);
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

  Param_Test::Fixed_Struct in_;
  // in parameter

  Param_Test::Fixed_Struct inout_;
  // inout parameter

  Param_Test::Fixed_Struct out_;
  // out parameter

  Param_Test::Fixed_Struct ret_;
  // return value
};

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of strings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_String_Sequence
{
public:
  Test_String_Sequence (void);
  // ctor

  ~Test_String_Sequence (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref,
                    CORBA::Environment &env);
  // run the SII test

  int add_args (CORBA::NVList_ptr &nvlist,
                CORBA::NVList_ptr &retval,
                CORBA::Environment &env);
  // add args to NVList for DII

  const char *opname (void) const;
  // return operation name

  void init_parameters (void);
  // set values for parameters

  void reset_parameters (void);
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

  Param_Test::StrSeq_var in_;
  // in parameter

  Param_Test::StrSeq_var inout_;
  // inout parameter

  Param_Test::StrSeq_var out_;
  // out parameter

  Param_Test::StrSeq_var ret_;
  // return value
};

#endif /* if !defined */
