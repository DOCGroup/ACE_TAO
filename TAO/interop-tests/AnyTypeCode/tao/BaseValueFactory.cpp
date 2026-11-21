#include "BaseValueFactory.h"
#include "BaseValueImpl.h"

CORBA::ValueBase *
Demo_BaseValueFactory::create_for_unmarshal ()
{
  return new Demo_BaseValueImpl;
}

Demo::BaseValue *
Demo_BaseValueFactory::create (char const *str)
{
  Demo_BaseValueImpl * const baseValueImpl = new Demo_BaseValueImpl;
  baseValueImpl->str (str);
  return baseValueImpl;
}
