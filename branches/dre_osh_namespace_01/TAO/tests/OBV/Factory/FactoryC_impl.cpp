// $Id$

#include "FactoryC_impl.h"

ACE_RCSID(Factory, FactoryC_impl, "$Id$")

OBV_FactoryTest::Value2*
Value2_init_impl::create_default (CORBA::ULong id)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  OBV_FactoryTest::Value2* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  0);

  if (ret_val)
    {
      ret_val->id (id);
    }

  return ret_val;
}

OBV_FactoryTest::Value2*
Value2_init_impl::create (
    CORBA::ULong id,
    const OBV_FactoryTest::BaseValue::BV_Data & data
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  OBV_FactoryTest::Value2* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  0);

  if (ret_val)
    {
      ret_val->id (id);
      ret_val->data (data);
    }

  return ret_val;
}

CORBA::ValueBase *
Value2_init_impl::create_for_unmarshal (void)
{
  OBV_FactoryTest::Value2* ret_val = 0;
  ACE_NEW_RETURN (ret_val,
                  Value2_impl,
                  0);

  return ret_val;
}
