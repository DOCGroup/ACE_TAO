
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
  Test_impl (CORBA::ORB_ptr orb);

  virtual OBV_FactoryTest::BaseValue * get_base_value ();

  virtual OBV_FactoryTest::Value1 * get_value1 ();

  virtual OBV_FactoryTest::Value2 * get_value2 ();

  virtual void shutdown ();

private:
  /// The ORB
  CORBA::ORB_var orb_;
};

#endif /* TAO_FACTORY_S_IMPL_H */
