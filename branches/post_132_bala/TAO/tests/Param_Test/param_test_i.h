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

  virtual Coffee::Desc * description (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // get attribute

  virtual void description (const Coffee::Desc &description
                            ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
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
                CORBA::ORB_ptr orb);
  // Constructor

  ~Param_Test_i (void);
  // Destructor

  virtual CORBA::Short test_short (CORBA::Short s1,
                                   CORBA::Short &s2,
                                   CORBA::Short_out s3
                                   ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for shorts

  virtual CORBA::ULongLong test_ulonglong (CORBA::ULongLong s1,
                                           CORBA::ULongLong &s2,
                                           CORBA::ULongLong_out s3
                                           ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for unsigned long long

  virtual char *test_unbounded_string (const char *s1,
                                       char *&s2,
                                       CORBA::String_out s3
                                       ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for unbounded strings

  virtual char *test_bounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3
                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for bounded strings

  virtual CORBA::WChar *test_unbounded_wstring (const CORBA::WChar *ws1,
                                                CORBA::WChar *&ws2,
                                                CORBA::WString_out ws3
                                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for unbounded wstrings

  virtual CORBA::WChar *test_bounded_wstring (const CORBA::WChar *ws1,
                                              CORBA::WChar *&ws2,
                                              CORBA::WString_out ws3
                                              ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for bounded wstrings

  virtual Param_Test::Fixed_Struct
  test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                     Param_Test::Fixed_Struct &s2,
                     Param_Test::Fixed_Struct_out s3
                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // test for fixed structures

  // = Start of sequences tests...
  virtual Param_Test::Long_Seq * test_long_sequence (
      const Param_Test::Long_Seq & s1,
      Param_Test::Long_Seq & s2,
      Param_Test::Long_Seq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_Long_Seq * test_bounded_long_sequence (
      const Param_Test::Bounded_Long_Seq & s1,
      Param_Test::Bounded_Long_Seq & s2,
      Param_Test::Bounded_Long_Seq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Short_Seq * test_short_sequence (
      const Param_Test::Short_Seq & s1,
      Param_Test::Short_Seq & s2,
      Param_Test::Short_Seq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_Short_Seq * test_bounded_short_sequence (
          const Param_Test::Bounded_Short_Seq & s1,
      Param_Test::Bounded_Short_Seq & s2,
      Param_Test::Bounded_Short_Seq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::StrSeq * test_strseq (
      const Param_Test::StrSeq & s1,
      Param_Test::StrSeq & s2,
      Param_Test::StrSeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_StrSeq * test_bounded_strseq (
      const Param_Test::Bounded_StrSeq & s1,
      Param_Test::Bounded_StrSeq & s2,
      Param_Test::Bounded_StrSeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::WStrSeq * test_wstrseq (
      const Param_Test::WStrSeq & ws1,
      Param_Test::WStrSeq & ws2,
      Param_Test::WStrSeq_out ws3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_WStrSeq * test_bounded_wstrseq (
      const Param_Test::Bounded_WStrSeq & ws1,
      Param_Test::Bounded_WStrSeq & ws2,
      Param_Test::Bounded_WStrSeq_out ws3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::StructSeq * test_struct_sequence (
      const Param_Test::StructSeq & s1,
      Param_Test::StructSeq & s2,
      Param_Test::StructSeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::PathSpec * test_unbounded_struct_sequence (
      const Param_Test::PathSpec & s1,
      Param_Test::PathSpec & s2,
      Param_Test::PathSpec_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_StructSeq * test_bounded_struct_sequence (
      const Param_Test::Bounded_StructSeq & s1,
      Param_Test::Bounded_StructSeq & s2,
      Param_Test::Bounded_StructSeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::ArraySeq * test_array_sequence (
      const Param_Test::ArraySeq & s1,
      Param_Test::ArraySeq & s2,
      Param_Test::ArraySeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_ArraySeq * test_bounded_array_sequence (
      const Param_Test::Bounded_ArraySeq & s1,
      Param_Test::Bounded_ArraySeq & s2,
      Param_Test::Bounded_ArraySeq_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Coffee_Mix * test_coffe_mix (
      const Param_Test::Coffee_Mix & s1,
      Param_Test::Coffee_Mix & s2,
      Param_Test::Coffee_Mix_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::Bounded_Coffee_Mix * test_bounded_coffe_mix (
      const Param_Test::Bounded_Coffee_Mix & s1,
      Param_Test::Bounded_Coffee_Mix & s2,
      Param_Test::Bounded_Coffee_Mix_out s3
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual Param_Test::AnySeq * test_anyseq (
          const Param_Test::AnySeq & s1,
          Param_Test::AnySeq & s2,
          Param_Test::AnySeq_out s3
          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // = End of sequences tests....

  virtual Param_Test::Var_Struct *
  test_var_struct (const Param_Test::Var_Struct &s1,
                   Param_Test::Var_Struct &s2,
                   Param_Test::Var_Struct_out s3
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for variable structs

  virtual Param_Test::Nested_Struct *
  test_nested_struct (const Param_Test::Nested_Struct &s1,
                      Param_Test::Nested_Struct &s2,
                      Param_Test::Nested_Struct_out s3
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for nested structs

  virtual Param_Test::Recursive_Struct *
    test_recursive_struct (const Param_Test::Recursive_Struct &rs1,
                           Param_Test::Recursive_Struct &rs2,
                           Param_Test::Recursive_Struct_out rs3
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for recursive structs

  virtual Param_Test::Objref_Struct *
  test_objref_struct (const Param_Test::Objref_Struct &s1,
                      Param_Test::Objref_Struct &s2,
                      Param_Test::Objref_Struct_out s3
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Coffee_ptr
  make_coffee (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // make a coffee object

  virtual Coffee_ptr
  test_objref (Coffee_ptr o1,
               Coffee_ptr &o2,
               Coffee_out o3
               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for object references

  virtual CORBA::TypeCode_ptr
  test_typecode (CORBA::TypeCode_ptr t1,
                 CORBA::TypeCode_ptr &o2,
                 CORBA::TypeCode_out o3
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for typecodes

  virtual CORBA::Any *
  test_any (const CORBA::Any &a1,
            CORBA::Any &a2,
            CORBA::Any_out a3
            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for Anys

  virtual Param_Test::Fixed_Array_slice *
  test_fixed_array (const Param_Test::Fixed_Array a1,
                    Param_Test::Fixed_Array a2,
                    Param_Test::Fixed_Array_out a3
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for fixed size arrays

  virtual Param_Test::Var_Array_slice *
  test_var_array (const Param_Test::Var_Array a1,
                  Param_Test::Var_Array a2,
                  Param_Test::Var_Array_out a3
                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // test for arrays of variable types

  virtual CORBA::ULong test_exception (CORBA::ULong s1,
                                       CORBA::ULong& s2,
                                       CORBA::ULong_out s3
                                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Param_Test::Ooops));

  virtual Param_Test::Big_Union*
    test_big_union (const Param_Test::Big_Union& u1,
                    Param_Test::Big_Union& u2,
                    Param_Test::Big_Union_out u3
                    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Param_Test::Small_Union
    test_small_union (const Param_Test::Small_Union& u1,
                      Param_Test::Small_Union& u2,
                      Param_Test::Small_Union_out u3
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual Param_Test::Recursive_Union*
    test_recursive_union (const Param_Test::Recursive_Union& ru1,
                          Param_Test::Recursive_Union& ru2,
                          Param_Test::Recursive_Union_out ru3
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Any*
    test_complex_any (const CORBA::Any &a1,
                      CORBA::Any &a2,
                      CORBA::Any_out a3
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Test for multi dimensional arrays
  virtual Param_Test::Multdim_Array_slice *
  test_multdim_array (const Param_Test::Multdim_Array,
                      Param_Test::Multdim_Array ,
                      Param_Test::Multdim_Array_out
                      ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));


private:
  CORBA::ORB_var orb_;
  // Remember out ORB.

  Coffee_i obj_;
  // the coffee object reference we maintain

  void throw_badboy (ACE_ENV_SINGLE_ARG_DECL);
  // Called by test_exception. This will avoid the compiler
  // warning that test_exception is throwing an exception
  // not in its THROW_SPEC, but still test TAO's
  // conversion of such an exception to UNKNOWN.
};

#endif /* PARAM_TEST_I_H */
