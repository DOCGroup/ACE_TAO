// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO/tests/OBV/Truncatable
//
// = FILENAME
//   TruncatableS_impl.h
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#ifndef TAO_TRUNCATABLE_S_IMPL_H
#define TAO_TRUNCATABLE_S_IMPL_H

#include "TruncatableS.h"

class Test_impl : public POA_OBV_TruncatableTest::Test
{
  // = TITLE
  //   Implementation of Test interface
  //
  // = DESCRIPTION
  //   This interface is provided to produce valuetypes and test
  //   marshaling
  //
public:
  Test_impl (CORBA::ORB_ptr orb);
  // ctor

  virtual void op1 (
        const char * id,
        ::OBV_TruncatableTest::BaseValue * iv,
        ::OBV_TruncatableTest::BaseValue_out ov,
        char *& desc
      );

  virtual void op2 (
        ::OBV_TruncatableTest::TValue1 * iv,
        const char * id,
        ::OBV_TruncatableTest::TValue1_out ov,
        char *& desc
      );

  virtual void op3 (
        const char * id,
        ::OBV_TruncatableTest::TValue4 * iv,
        ::OBV_TruncatableTest::TValue4_out ov,
        char *& desc
      );

  virtual ::OBV_TruncatableTest::BaseValue * op4 (
      const char * id,
      ::OBV_TruncatableTest::TValue1 * iv1,
      ::CORBA::Short x,
      ::OBV_TruncatableTest::TValue4 * iv2,
      ::OBV_TruncatableTest::TValue4 * iv3,
      ::OBV_TruncatableTest::TValue1 * iv4,
      char *& desc
    );

  virtual void op5 (const CORBA::Any& val,
                    const char * id,
                    ::OBV_TruncatableTest::TValue1_out ov,
                    char *& desc
 );

  virtual void shutdown (void);

private:
  CORBA::ORB_var orb_;
  // The ORB
};

#endif /* TAO_TRUNCATABLE_S_IMPL_H */
