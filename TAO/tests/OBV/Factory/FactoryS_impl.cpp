#include "FactoryC_impl.h"
#include "FactoryS_impl.h"

Test_impl::Test_impl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

OBV_FactoryTest::BaseValue *
Test_impl::get_base_value ()
{
  OBV_FactoryTest::BaseValue *ret_val = nullptr;
  ACE_NEW_RETURN (ret_val,
                  OBV_OBV_FactoryTest::BaseValue,
                  nullptr);

  return ret_val;
}

OBV_FactoryTest::Value1 *
Test_impl::get_value1 ()
{
  OBV_FactoryTest::Value1 *ret_val = nullptr;
  ACE_NEW_RETURN (ret_val,
                  OBV_OBV_FactoryTest::Value1,
                  nullptr);

  return ret_val;
}

OBV_FactoryTest::Value2 *
Test_impl::get_value2 ()
{
  OBV_FactoryTest::Value2 *ret_val = nullptr;
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  nullptr);

  return ret_val;
}

void
Test_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
