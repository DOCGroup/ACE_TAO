// $Id

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    param_test_i.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (PARAM_TEST_I_H)
#define	PARAM_TEST_I_H

#include "param_testS.h"

class Param_Test_i : public POA_Param_Test
{
  // = TITLE
  //    Param_Test_i
  // = DESCRIPTION
  //    Implementation of the Param_Test test suite.
public:
  Param_Test_i (const char *obj_name = 0);
  // Constructor

  ~Param_Test_i (void);
  // Destructor

  virtual CORBA::Short test_short (CORBA::Short s1,
                                   CORBA::Short &s2,
                                   CORBA::Short_out s3,
                                   CORBA::Environment &env);
  // test for shorts

  virtual char *test_unbounded_string (const char *s1,
                                       char *&s2,
                                       CORBA::String_out s3,
                                       CORBA::Environment &env);
  // test for unbounded strings

  virtual Param_Test::Fixed_Struct
  test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                     Param_Test::Fixed_Struct &s2,
                     Param_Test::Fixed_Struct_out s3,
                     CORBA::Environment &env);
  // test for fixed structures

  virtual Param_Test::StrSeq *
  test_strseq (const Param_Test::StrSeq &s1,
               Param_Test::StrSeq &s2,
               Param_Test::StrSeq_out s3,
               CORBA::Environment &env);
  // test for string sequence

  virtual Param_Test::Var_Struct *
  test_var_struct (const Param_Test::Var_Struct &s1,
                   Param_Test::Var_Struct &s2,
                   Param_Test::Var_Struct_out s3,
                   CORBA::Environment &env);
  // test for variable structs

  virtual Param_Test::Nested_Struct *
  test_nested_struct (const Param_Test::Nested_Struct &s1,
                      Param_Test::Nested_Struct &s2,
                      Param_Test::Nested_Struct_out s3,
                      CORBA::Environment &env);
  // test for nested structs
};

#endif /* PARAM_TEST_I_H */
