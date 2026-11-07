
//=============================================================================
/**
 *  @file   FactoryS_impl.h
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================


#ifndef TAO_FACTORY_S_IMPL_H
#define TAO_FACTORY_S_IMPL_H

#include "FactoryS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl : public POA_OBV_FactoryTest::Test
{
public:
  /// ctor
  explicit Test_impl (CORBA::ORB_ptr orb);

  Test_impl (Test_impl const&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (Test_impl const&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;

  OBV_FactoryTest::BaseValue *get_base_value () override;

  OBV_FactoryTest::Value1 *get_value1 () override;

  OBV_FactoryTest::Value2 *get_value2 () override;

  void shutdown () override;

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_FACTORY_S_IMPL_H */
