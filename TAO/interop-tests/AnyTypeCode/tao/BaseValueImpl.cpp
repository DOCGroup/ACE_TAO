#include "BaseValueImpl.h"

Demo_BaseValueImpl::Demo_BaseValueImpl()
{
}

Demo_BaseValueImpl::~Demo_BaseValueImpl()
{
}

CORBA::ValueBase*
Demo_BaseValueImpl::_copy_value()
{
  Demo_BaseValueImpl *baseValueImpl = new Demo_BaseValueImpl ();
  baseValueImpl->str (str ());
  return baseValueImpl;
}

