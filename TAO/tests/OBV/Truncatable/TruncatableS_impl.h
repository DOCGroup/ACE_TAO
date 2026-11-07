
//=============================================================================
/**
 *  @file   TruncatableS_impl.h
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef TAO_TRUNCATABLE_S_IMPL_H
#define TAO_TRUNCATABLE_S_IMPL_H

#include "TruncatableS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl : public POA_OBV_TruncatableTest::Test
{
public:
  /// ctor
  explicit Test_impl (CORBA::ORB_ptr orb);

  Test_impl (Test_impl const&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (Test_impl const&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;

  void op1 (char const *id,
            ::OBV_TruncatableTest::BaseValue *iv,
            ::OBV_TruncatableTest::BaseValue_out ov,
            char *&desc) override;

  void op2 (::OBV_TruncatableTest::TValue1 *iv,
            char const *id,
            ::OBV_TruncatableTest::TValue1_out ov,
            char *&desc) override;

  void op3 (char const *id,
            ::OBV_TruncatableTest::TValue4 *iv,
            ::OBV_TruncatableTest::TValue4_out ov,
            char *&desc) override;

  ::OBV_TruncatableTest::BaseValue *op4 (
      char const *id,
      ::OBV_TruncatableTest::TValue1 *iv1,
      ::CORBA::Short x,
      ::OBV_TruncatableTest::TValue4 *iv2,
      ::OBV_TruncatableTest::TValue4 *iv3,
      ::OBV_TruncatableTest::TValue1 *iv4,
      char *&desc) override;

  void op5 (CORBA::Any const &val,
            char const *id,
            ::OBV_TruncatableTest::TValue1_out ov,
            char *&desc) override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_TRUNCATABLE_S_IMPL_H */
