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

// Implementation of the Coffee interface
class Coffee_i : public POA_Coffee

{
public:
  Coffee_i (const char *name);
  // constructor

  ~Coffee_i (void);
  // destructor

  // =methods for the attribute

  virtual Coffee::Desc * description (CORBA::Environment &env);
  // get attribute

  virtual void description (const Coffee::Desc &description,
                            CORBA::Environment &env);
  // set attribute

private:
  CORBA::String_var name_;
  // my description
};

// the implementation of the Param_test interface
class Param_Test_i : public POA_Param_Test
{
  // = TITLE
  //    Param_Test_i
  // = DESCRIPTION
  //    Implementation of the Param_Test test suite.
public:
  Param_Test_i (const char *coffee_name,
                const char *obj_name = 0);
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

  virtual Param_Test::StructSeq *
  test_struct_sequence (const Param_Test::StructSeq &s1,
                        Param_Test::StructSeq &s2,
                        Param_Test::StructSeq_out s3,
                        CORBA::Environment &env);
  // test for struct sequences

  virtual Coffee_ptr
  make_coffee (CORBA::Environment &env);
  // make a coffee object

  virtual Coffee_ptr
  test_objref (Coffee_ptr o1,
               Coffee_ptr &o2,
               Coffee_out o3,
               CORBA::Environment &env);
  // test for object references

private:
  Coffee_i obj_;
  // the coffee object reference we maintain
};

#endif /* PARAM_TEST_I_H */
