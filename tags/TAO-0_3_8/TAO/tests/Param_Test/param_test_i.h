// $Id$

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

#ifndef PARAM_TEST_I_H
#define PARAM_TEST_I_H

#include "param_testSer.h"

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

  virtual CORBA::ULongLong test_ulonglong (CORBA::ULongLong s1,
                                           CORBA::ULongLong &s2,
                                           CORBA::ULongLong_out s3,
                                           CORBA::Environment &env);
  // test for unsigned long long

  virtual char *test_unbounded_string (const char *s1,
                                       char *&s2,
                                       CORBA::String_out s3,
                                       CORBA::Environment &env);
  // test for unbounded strings

  virtual char *test_bounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3,
                                     CORBA::Environment &env);
  // test for bounded strings

  virtual Param_Test::Fixed_Struct
  test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                     Param_Test::Fixed_Struct &s2,
                     Param_Test::Fixed_Struct_out s3,
                     CORBA::Environment &env);
  // test for fixed structures

  // = Start of sequences tests...
  virtual Param_Test::Long_Seq * test_long_sequence (
      const Param_Test::Long_Seq & s1,
      Param_Test::Long_Seq & s2,
      Param_Test::Long_Seq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Bounded_Long_Seq * test_bounded_long_sequence (
      const Param_Test::Bounded_Long_Seq & s1,
      Param_Test::Bounded_Long_Seq & s2,
      Param_Test::Bounded_Long_Seq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Short_Seq * test_short_sequence (
          const Param_Test::Short_Seq & s1,
      Param_Test::Short_Seq & s2,
      Param_Test::Short_Seq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Bounded_Short_Seq * test_bounded_short_sequence (
          const Param_Test::Bounded_Short_Seq & s1,
      Param_Test::Bounded_Short_Seq & s2,
      Param_Test::Bounded_Short_Seq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::StrSeq * test_strseq (
      const Param_Test::StrSeq & s1,
      Param_Test::StrSeq & s2,
      Param_Test::StrSeq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Bounded_StrSeq * test_bounded_strseq (
      const Param_Test::Bounded_StrSeq & s1,
      Param_Test::Bounded_StrSeq & s2,
      Param_Test::Bounded_StrSeq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::StructSeq * test_struct_sequence (
      const Param_Test::StructSeq & s1,
      Param_Test::StructSeq & s2,
      Param_Test::StructSeq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::PathSpec * test_unbounded_struct_sequence (
      const Param_Test::PathSpec & s1,
      Param_Test::PathSpec & s2,
      Param_Test::PathSpec_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Bounded_StructSeq * test_bounded_struct_sequence (
      const Param_Test::Bounded_StructSeq & s1,
      Param_Test::Bounded_StructSeq & s2,
      Param_Test::Bounded_StructSeq_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Coffee_Mix * test_coffe_mix (
      const Param_Test::Coffee_Mix & s1,
      Param_Test::Coffee_Mix & s2,
      Param_Test::Coffee_Mix_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::Bounded_Coffee_Mix * test_bounded_coffe_mix (
      const Param_Test::Bounded_Coffee_Mix & s1,
      Param_Test::Bounded_Coffee_Mix & s2,
      Param_Test::Bounded_Coffee_Mix_out s3,
      CORBA::Environment &env
     );
  virtual Param_Test::AnySeq * test_anyseq (
          const Param_Test::AnySeq & s1,
          Param_Test::AnySeq & s2,
          Param_Test::AnySeq_out s3,
          CORBA::Environment &env
          );
  // = End of sequences tests....

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

  virtual Param_Test::Objref_Struct *
  test_objref_struct (const Param_Test::Objref_Struct &s1,
                      Param_Test::Objref_Struct &s2,
                      Param_Test::Objref_Struct_out s3,
                      CORBA::Environment &env);

  virtual Coffee_ptr
  make_coffee (CORBA::Environment &env);
  // make a coffee object

  virtual Coffee_ptr
  test_objref (Coffee_ptr o1,
               Coffee_ptr &o2,
               Coffee_out o3,
               CORBA::Environment &env);
  // test for object references

  virtual CORBA::TypeCode_ptr
  test_typecode (CORBA::TypeCode_ptr t1,
                 CORBA::TypeCode_ptr &o2,
                 CORBA::TypeCode_out o3,
                 CORBA::Environment &env);
  // test for typecodes

  virtual CORBA::Any *
  test_any (const CORBA::Any &a1,
            CORBA::Any &a2,
            CORBA::Any_out a3,
            CORBA::Environment &env);
  // test for Anys

  virtual Param_Test::Fixed_Array_slice *
  test_fixed_array (const Param_Test::Fixed_Array a1,
                    Param_Test::Fixed_Array a2,
                    Param_Test::Fixed_Array_out a3,
                    CORBA::Environment &env);
  // test for fixed size arrays

  virtual Param_Test::Var_Array_slice *
  test_var_array (const Param_Test::Var_Array a1,
                  Param_Test::Var_Array a2,
                  Param_Test::Var_Array_out a3,
                  CORBA::Environment &env);
  // test for arrays of variable types

  virtual CORBA::ULong test_exception (CORBA::ULong s1,
                                       CORBA::ULong& s2,
                                       CORBA::ULong_out s3,
                                       CORBA::Environment &env);

  void shutdown (CORBA::Environment &env);

  virtual Param_Test::Big_Union*
      test_big_union (const Param_Test::Big_Union& u1,
                      Param_Test::Big_Union& u2,
                      Param_Test::Big_Union_out u3,
                      CORBA::Environment &env);

private:
  Coffee_i obj_;
  // the coffee object reference we maintain

  int test_exception_count_;
  // Count the number of calls to test_exception() so we can throw
  // every 3 calls or so.
};

#endif /* PARAM_TEST_I_H */
