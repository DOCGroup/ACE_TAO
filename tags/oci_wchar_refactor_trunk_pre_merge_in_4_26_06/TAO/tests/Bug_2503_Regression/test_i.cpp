// $Id$

#include "test_i.h"

#include "tao/Utils/Servant_Var.h"

test_i::
test_i()
{
}

void test_i::
the_operation(CORBA::Long & x)
  throw(CORBA::SystemException)
{
  x = 42;
}

char * test_i::
create_and_activate_server(CORBA::ORB_ptr orb)
{
  TAO::Utils::Servant_Var<test_i> impl(
      new test_i);

  Test_var ref = impl->_this();

  return orb->object_to_string(ref.in());
}
