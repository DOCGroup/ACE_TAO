
//=============================================================================
/**
 *  @file    param_test_i.h
 *
 *  $Id$
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef PARAM_TEST_I_H
#define PARAM_TEST_I_H

#include "param_testS.h"

// Implementation of the Coffee interface
class Coffee_i : public POA_Coffee

{
public:
  /// constructor
  Coffee_i (const char *name);

  /// destructor
  ~Coffee_i (void);

  // =methods for the attribute

  /// get attribute
  virtual Coffee::Desc * description (void);

  /// set attribute
  virtual void description (const Coffee::Desc &description);

private:
  /// my description
  CORBA::String_var name_;
};

// the implementation of the Param_test interface
/**
 * @class Param_Test_i
 *
 * @brief Param_Test_i
 *
 * Implementation of the Param_Test test suite.
 */
class Param_Test_i : public POA_Param_Test
{
public:
  /// Constructor
  Param_Test_i (const char *coffee_name,
                CORBA::ORB_ptr orb);

  /// Destructor
  ~Param_Test_i (void);

  /// test for shorts
  virtual CORBA::Short test_short (CORBA::Short s1,
                                   CORBA::Short &s2,
                                   CORBA::Short_out s3);

  /// test for unsigned long long
  virtual CORBA::ULongLong test_ulonglong (CORBA::ULongLong s1,
                                           CORBA::ULongLong &s2,
                                           CORBA::ULongLong_out s3);

  /// test for unbounded strings
  virtual char *test_unbounded_string (const char *s1,
                                       char *&s2,
                                       CORBA::String_out s3);

  /// test for bounded strings
  virtual char *test_bounded_string (const char *s1,
                                     char *&s2,
                                     CORBA::String_out s3);

  /// test for unbounded wstrings
  virtual CORBA::WChar *test_unbounded_wstring (const CORBA::WChar *ws1,
                                                CORBA::WChar *&ws2,
                                                CORBA::WString_out ws3);

  /// test for bounded wstrings
  virtual CORBA::WChar *test_bounded_wstring (const CORBA::WChar *ws1,
                                              CORBA::WChar *&ws2,
                                              CORBA::WString_out ws3);

  /// test for fixed structures
  virtual Param_Test::Fixed_Struct
  test_fixed_struct (const Param_Test::Fixed_Struct &s1,
                     Param_Test::Fixed_Struct &s2,
                     Param_Test::Fixed_Struct_out s3);

  // = Start of sequences tests...
  /// = End of sequences tests....
  virtual CORBA::LongSeq * test_long_sequence (
      const CORBA::LongSeq & s1,
      CORBA::LongSeq & s2,
      CORBA::LongSeq_out s3);
  virtual Param_Test::Bounded_Long_Seq * test_bounded_long_sequence (
      const Param_Test::Bounded_Long_Seq & s1,
      Param_Test::Bounded_Long_Seq & s2,
      Param_Test::Bounded_Long_Seq_out s3);
  virtual CORBA::ShortSeq * test_short_sequence (
      const CORBA::ShortSeq & s1,
      CORBA::ShortSeq & s2,
      CORBA::ShortSeq_out s3);
  virtual Param_Test::Bounded_Short_Seq * test_bounded_short_sequence (
          const Param_Test::Bounded_Short_Seq & s1,
      Param_Test::Bounded_Short_Seq & s2,
      Param_Test::Bounded_Short_Seq_out s3);
  virtual CORBA::StringSeq * test_strseq (
      const CORBA::StringSeq & s1,
      CORBA::StringSeq & s2,
      CORBA::StringSeq_out s3);
  virtual Param_Test::Bounded_StrSeq * test_bounded_strseq (
      const Param_Test::Bounded_StrSeq & s1,
      Param_Test::Bounded_StrSeq & s2,
      Param_Test::Bounded_StrSeq_out s3);
  virtual CORBA::WStringSeq * test_wstrseq (
      const CORBA::WStringSeq & ws1,
      CORBA::WStringSeq & ws2,
      CORBA::WStringSeq_out ws3);
  virtual Param_Test::Bounded_WStrSeq * test_bounded_wstrseq (
      const Param_Test::Bounded_WStrSeq & ws1,
      Param_Test::Bounded_WStrSeq & ws2,
      Param_Test::Bounded_WStrSeq_out ws3);
  virtual Param_Test::StructSeq * test_struct_sequence (
      const Param_Test::StructSeq & s1,
      Param_Test::StructSeq & s2,
      Param_Test::StructSeq_out s3);
  virtual Param_Test::PathSpec * test_unbounded_struct_sequence (
      const Param_Test::PathSpec & s1,
      Param_Test::PathSpec & s2,
      Param_Test::PathSpec_out s3);
  virtual Param_Test::Bounded_StructSeq * test_bounded_struct_sequence (
      const Param_Test::Bounded_StructSeq & s1,
      Param_Test::Bounded_StructSeq & s2,
      Param_Test::Bounded_StructSeq_out s3);
  virtual Param_Test::ArraySeq * test_array_sequence (
      const Param_Test::ArraySeq & s1,
      Param_Test::ArraySeq & s2,
      Param_Test::ArraySeq_out s3);
  virtual Param_Test::Bounded_ArraySeq * test_bounded_array_sequence (
      const Param_Test::Bounded_ArraySeq & s1,
      Param_Test::Bounded_ArraySeq & s2,
      Param_Test::Bounded_ArraySeq_out s3);
  virtual Param_Test::Coffee_Mix * test_coffe_mix (
      const Param_Test::Coffee_Mix & s1,
      Param_Test::Coffee_Mix & s2,
      Param_Test::Coffee_Mix_out s3);
  virtual Param_Test::Bounded_Coffee_Mix * test_bounded_coffe_mix (
      const Param_Test::Bounded_Coffee_Mix & s1,
      Param_Test::Bounded_Coffee_Mix & s2,
      Param_Test::Bounded_Coffee_Mix_out s3);
  virtual CORBA::AnySeq * test_anyseq (
          const CORBA::AnySeq & s1,
          CORBA::AnySeq & s2,
          CORBA::AnySeq_out s3);

  /// test for variable structs
  virtual Param_Test::Var_Struct *
  test_var_struct (const Param_Test::Var_Struct &s1,
                   Param_Test::Var_Struct &s2,
                   Param_Test::Var_Struct_out s3);

  /// test for nested structs
  virtual Param_Test::Nested_Struct *
  test_nested_struct (const Param_Test::Nested_Struct &s1,
                      Param_Test::Nested_Struct &s2,
                      Param_Test::Nested_Struct_out s3);

  /// test for recursive structs
  virtual Param_Test::Recursive_Struct *
    test_recursive_struct (const Param_Test::Recursive_Struct &rs1,
                           Param_Test::Recursive_Struct &rs2,
                           Param_Test::Recursive_Struct_out rs3);

  virtual Param_Test::Objref_Struct *
  test_objref_struct (const Param_Test::Objref_Struct &s1,
                      Param_Test::Objref_Struct &s2,
                      Param_Test::Objref_Struct_out s3);

  /// make a coffee object
  virtual Coffee_ptr
  make_coffee (void);

  /// test for object references
  virtual Coffee_ptr
  test_objref (Coffee_ptr o1,
               Coffee_ptr &o2,
               Coffee_out o3);

  /// test for typecodes
  virtual CORBA::TypeCode_ptr
  test_typecode (CORBA::TypeCode_ptr t1,
                 CORBA::TypeCode_ptr &o2,
                 CORBA::TypeCode_out o3);

  /// test for Anys
  virtual CORBA::Any *
  test_any (const CORBA::Any &a1,
            CORBA::Any &a2,
            CORBA::Any_out a3);

  /// test for fixed size arrays
  virtual Param_Test::Fixed_Array_slice *
  test_fixed_array (const Param_Test::Fixed_Array a1,
                    Param_Test::Fixed_Array a2,
                    Param_Test::Fixed_Array_out a3);

  /// test for arrays of variable types
  virtual Param_Test::Var_Array_slice *
  test_var_array (const Param_Test::Var_Array a1,
                  Param_Test::Var_Array a2,
                  Param_Test::Var_Array_out a3);

  virtual CORBA::ULong test_exception (CORBA::ULong s1,
                                       CORBA::ULong& s2,
                                       CORBA::ULong_out s3);

  virtual Param_Test::Big_Union*
    test_big_union (const Param_Test::Big_Union& u1,
                    Param_Test::Big_Union& u2,
                    Param_Test::Big_Union_out u3);

  virtual Param_Test::Small_Union
    test_small_union (const Param_Test::Small_Union& u1,
                      Param_Test::Small_Union& u2,
                      Param_Test::Small_Union_out u3);

  virtual Param_Test::Recursive_Union*
    test_recursive_union (const Param_Test::Recursive_Union& ru1,
                          Param_Test::Recursive_Union& ru2,
                          Param_Test::Recursive_Union_out ru3);

  virtual CORBA::Any*
    test_complex_any (const CORBA::Any &a1,
                      CORBA::Any &a2,
                      CORBA::Any_out a3);

  // Test for multi dimensional arrays
  virtual Param_Test::Multdim_Array_slice *
  test_multdim_array (const Param_Test::Multdim_Array,
                      Param_Test::Multdim_Array ,
                      Param_Test::Multdim_Array_out);

  void shutdown (void);


private:
  /// Remember out ORB.
  CORBA::ORB_var orb_;

  /// the coffee object reference we maintain
  Coffee_i obj_;

  /**
   * Called by test_exception. This will avoid the compiler
   * warning that test_exception is throwing an exception
   * not in its THROW_SPEC, but still test TAO's
   * conversion of such an exception to UNKNOWN.
   */
  void throw_badboy (void);
};

#endif /* PARAM_TEST_I_H */
