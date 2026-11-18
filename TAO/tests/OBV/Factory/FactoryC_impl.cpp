#include "FactoryC_impl.h"

OBV_FactoryTest::Value2 *
Value2_init_impl::create_default (CORBA::ULong id)
{
  OBV_FactoryTest::Value2 *ret_val {};
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  nullptr);

  if (ret_val != nullptr)
    {
      ret_val->id (id);
    }

  return ret_val;
}

OBV_FactoryTest::Value2 *
Value2_init_impl::create (
    CORBA::ULong id,
    OBV_FactoryTest::BaseValue::BV_Data const &data)
{
  OBV_FactoryTest::Value2 *ret_val {};
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  nullptr);

  if (ret_val != nullptr)
    {
      ret_val->id (id);
      ret_val->data (data);
    }

  return ret_val;
}

CORBA::ValueBase *
Value2_init_impl::create_for_unmarshal ()
{
  OBV_FactoryTest::Value2 *ret_val {};
  ACE_NEW_THROW_EX (ret_val,
                    Value2_impl,
                    CORBA::NO_MEMORY ());

  return ret_val;
}

CORBA::ValueBase *
ApplicationFactoryValue_impl::_copy_value ()
{
  ApplicationFactoryValue_impl *ret_val {};
  ACE_NEW_THROW_EX (ret_val,
                    ApplicationFactoryValue_impl,
                    CORBA::NO_MEMORY ());
  ret_val->value (this->value ());
  ret_val->application_state (this->application_state ());
  return ret_val;
}

OBV_FactoryTest::ApplicationFactoryValue *
ApplicationFactoryValue_init_impl::create ()
{
  ApplicationFactoryValue_impl *ret_val {};
  ACE_NEW_RETURN (ret_val, ApplicationFactoryValue_impl, nullptr);
  return ret_val;
}

CORBA::ValueBase *
ApplicationFactoryValue_init_impl::create_for_unmarshal ()
{
  ApplicationFactoryValue_impl *ret_val {};
  ACE_NEW_THROW_EX (ret_val,
                    ApplicationFactoryValue_impl,
                    CORBA::NO_MEMORY ());
  return ret_val;
}
