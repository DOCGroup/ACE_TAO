// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/ValueBox
//
// = FILENAME
//   Test_impl.h
//
// = AUTHOR
//   Gary Maxey
//
// ============================================================================


#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "valueboxS.h"


class Test_impl : public POA_Test
{
public:
  Test_impl (CORBA::ORB_ptr orb);
  // ctor

  virtual VBlong * basic_op1 (
                       VBlong * p1,
                       VBlong *& p2,
                       VBlong_out p3
                       ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual vb_basic::M_VBlong * basic_op2(vb_basic::M_VBlong * p1,
                                         vb_basic::M_VBlong *& p2,
                                         vb_basic::M_VBlong_out p3
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::CORBA::Long basic_op3 (
                                   ::CORBA::Long p1,
                                   ::CORBA::Long& p2,
                                   ::CORBA::Long_out p3
                                   ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual VBstring * string_op1 (::VBstring * p1,
                                 ::VBstring *& p2,
                                 ::VBstring_out p3
                                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual char * string_op2 (const char * p1,
                             char *& p2,
                             CORBA::String_out p3
                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBseqlong * seq_op1 (::VBseqlong * p1,
                                 ::VBseqlong *& p2,
                                 ::VBseqlong_out p3
                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void seq_op2 (const ::TDseqlong & p1,
                        ::TDseqlong & p2,
                        ::TDseqlong_out p3
                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBfixed_struct1 * struct_op1 (
                                          ::VBfixed_struct1 * p1,
                                          ::VBfixed_struct1 *& p2,
                                          ::VBfixed_struct1_out p3
                                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void struct_op2 (const ::Fixed_Struct1 & p1,
                           ::Fixed_Struct1 & p2,
                           ::Fixed_Struct1_out p3
                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBvariable_struct1 * struct_op3 (::VBvariable_struct1 * p1,
                                             ::VBvariable_struct1 *& p2,
                                             ::VBvariable_struct1_out p3
                                             ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));


  virtual void struct_op4 (const ::Variable_Struct1 & p1,
                           ::Variable_Struct1 & p2,
                           ::Variable_Struct1_out p3
                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBlongarray * array_op1 (::VBlongarray * p1,
                                     ::VBlongarray *& p2,
                                     ::VBlongarray_out p3
                                     ACE_ENV_ARG_DECL_NOT_USED)
     ACE_THROW_SPEC ((CORBA::SystemException));


  virtual void array_op2 (const ::LongArray p1,
                          ::LongArray p2,
                          ::LongArray_out p3
                          ACE_ENV_ARG_DECL_NOT_USED)
     ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBstringarray * array_op3 (::VBstringarray * p1,
                                       ::VBstringarray *& p2,
                                       ::VBstringarray_out p3
                                     ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void array_op4 (const ::StringArray p1,
                          ::StringArray p2,
                          ::StringArray_out p3
                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBfixed_union1 * union_op1 (::VBfixed_union1 * p1,
                                        ::VBfixed_union1 *& p2,
                                        ::VBfixed_union1_out p3
                                        ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void union_op2 (const ::Fixed_Union1 & p1,
                          ::Fixed_Union1 & p2,
                          ::Fixed_Union1_out p3
                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual ::VBvariable_union1 * union_op3 (::VBvariable_union1 * p1,
                                           ::VBvariable_union1 *& p2,
                                           ::VBvariable_union1_out p3
                                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void union_op4 (const ::Variable_Union1 & p1,
                          ::Variable_Union1 & p2,
                          ::Variable_Union1_out p3
                          ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));


  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_TEST_IMPL_H */
