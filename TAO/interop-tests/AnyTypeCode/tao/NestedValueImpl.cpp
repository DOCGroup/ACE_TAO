//$Id$

#include "NestedValueImpl.h"

Demo_NestedValueImpl::Demo_NestedValueImpl (void)
{
}

Demo_NestedValueImpl::~Demo_NestedValueImpl (void)
{
}

CORBA::ValueBase*
Demo_NestedValueImpl::_copy_value (void)
{
  Demo_NestedValueImpl *nestedValueImpl = new Demo_NestedValueImpl();
  nestedValueImpl->foo1 (foo1 ());
  nestedValueImpl->foo2 (foo2 ());
  return nestedValueImpl;
}
