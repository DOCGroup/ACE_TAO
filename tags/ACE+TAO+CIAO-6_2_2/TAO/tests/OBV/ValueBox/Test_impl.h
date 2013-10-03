
//=============================================================================
/**
 *  @file   Test_impl.h
 *
 *  $Id$
 *
 *  @author Gary Maxey
 */
//=============================================================================



#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "valueboxS.h"


class Test_impl : public POA_Test
{
public:
  /// ctor
  Test_impl (CORBA::ORB_ptr orb);

  virtual VBlong * basic_op1 (
                       VBlong * p1,
                       VBlong *& p2,
                       VBlong_out p3);

  virtual vb_basic::M_VBlong * basic_op2(vb_basic::M_VBlong * p1,
                                         vb_basic::M_VBlong *& p2,
                                         vb_basic::M_VBlong_out p3);

  virtual ::CORBA::Long basic_op3 (
                                   ::CORBA::Long p1,
                                   ::CORBA::Long& p2,
                                   ::CORBA::Long_out p3);

  virtual VBstring * string_op1 (::VBstring * p1,
                                 ::VBstring *& p2,
                                 ::VBstring_out p3);

  virtual char * string_op2 (const char * p1,
                             char *& p2,
                             CORBA::String_out p3);

  virtual ::VBseqlong * seq_op1 (::VBseqlong * p1,
                                 ::VBseqlong *& p2,
                                 ::VBseqlong_out p3);

  virtual void seq_op2 (const ::TDseqlong & p1,
                        ::TDseqlong & p2,
                        ::TDseqlong_out p3);

  virtual ::VBfixed_struct1 * struct_op1 (
                                          ::VBfixed_struct1 * p1,
                                          ::VBfixed_struct1 *& p2,
                                          ::VBfixed_struct1_out p3);

  virtual void struct_op2 (const ::Fixed_Struct1 & p1,
                           ::Fixed_Struct1 & p2,
                           ::Fixed_Struct1_out p3);

  virtual ::VBvariable_struct1 * struct_op3 (::VBvariable_struct1 * p1,
                                             ::VBvariable_struct1 *& p2,
                                             ::VBvariable_struct1_out p3);


  virtual void struct_op4 (const ::Variable_Struct1 & p1,
                           ::Variable_Struct1 & p2,
                           ::Variable_Struct1_out p3);

  virtual ::VBlongarray * array_op1 (::VBlongarray * p1,
                                     ::VBlongarray *& p2,
                                     ::VBlongarray_out p3);


  virtual void array_op2 (const ::LongArray p1,
                          ::LongArray p2,
                          ::LongArray_out p3);

  virtual ::VBstringarray * array_op3 (::VBstringarray * p1,
                                       ::VBstringarray *& p2,
                                       ::VBstringarray_out p3);

  virtual void array_op4 (const ::StringArray p1,
                          ::StringArray p2,
                          ::StringArray_out p3);

  virtual ::VBfixed_union1 * union_op1 (::VBfixed_union1 * p1,
                                        ::VBfixed_union1 *& p2,
                                        ::VBfixed_union1_out p3);

  virtual void union_op2 (const ::Fixed_Union1 & p1,
                          ::Fixed_Union1 & p2,
                          ::Fixed_Union1_out p3);

  virtual ::VBvariable_union1 * union_op3 (::VBvariable_union1 * p1,
                                           ::VBvariable_union1 *& p2,
                                           ::VBvariable_union1_out p3);

  virtual void union_op4 (const ::Variable_Union1 & p1,
                          ::Variable_Union1 & p2,
                          ::Variable_Union1_out p3);


  virtual void shutdown (void);

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_TEST_IMPL_H */
