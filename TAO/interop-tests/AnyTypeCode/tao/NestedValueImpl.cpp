#include "NestedValueImpl.h"

Demo_NestedValueImpl::Demo_NestedValueImpl ()
{
}

Demo_NestedValueImpl::~Demo_NestedValueImpl ()
{
}

CORBA::ValueBase*
Demo_NestedValueImpl::_copy_value ()
{
  Demo_NestedValueImpl *nestedValueImpl = new Demo_NestedValueImpl();
  nestedValueImpl->foo1 (foo1 ());
  nestedValueImpl->foo2 (foo2 ());
  return nestedValueImpl;
}
