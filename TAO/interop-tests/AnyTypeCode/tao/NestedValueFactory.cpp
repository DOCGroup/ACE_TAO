#include "NestedValueFactory.h"
#include "NestedValueImpl.h"

CORBA::ValueBase*
Demo_NestedValueFactory::create_for_unmarshal (void)
{
  return new Demo_NestedValueImpl ();
}
