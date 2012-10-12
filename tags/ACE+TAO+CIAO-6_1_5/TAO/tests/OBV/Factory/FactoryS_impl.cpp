// $Id$

#include "FactoryC_impl.h"
#include "FactoryS_impl.h"

Test_impl::Test_impl (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
{
}

OBV_FactoryTest::BaseValue *
Test_impl::get_base_value (void)
{
  OBV_FactoryTest::BaseValue* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  OBV_OBV_FactoryTest::BaseValue,
                  0);

  return ret_val;
}

OBV_FactoryTest::Value1 *
Test_impl::get_value1 (void)
{
  OBV_FactoryTest::Value1* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  OBV_OBV_FactoryTest::Value1,
                  0);

  return ret_val;
}

OBV_FactoryTest::Value2 *
Test_impl::get_value2 (void)
{
  OBV_FactoryTest::Value2* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  0);

  return ret_val;
}

void
Test_impl::shutdown (void)
{
  this->orb_->shutdown (0);
}
