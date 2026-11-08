#include "BaseValueImpl.h"

CORBA::ValueBase*
Demo_BaseValueImpl::_copy_value()
{
  Demo_BaseValueImpl * const baseValueImpl = new Demo_BaseValueImpl;
  baseValueImpl->str (str ());
  return baseValueImpl;
}

