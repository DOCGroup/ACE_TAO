// $Id$

#include "test_i.h"

#include "tao/Utils/Servant_Var.h"

test_i::
test_i(CORBA::ORB_ptr orb) 
{
 this->orb_ = CORBA::ORB::_duplicate (orb);
}

void test_i::
the_operation(CORBA::Long & x)
  throw(CORBA::SystemException)
{
  x = 42;
}

char *
test_i::
create_and_activate_server()
{
  TAO::Utils::Servant_Var<test_i> impl(
      new test_i (this->orb_.in ()));

  Test_var ref = impl->_this();

  return this->orb_->object_to_string(ref.in());
}

void
test_i::shutdown (void)
  throw (CORBA::SystemException)
{
  if (!CORBA::is_nil (this->orb_.in ()))
    this->orb_->shutdown (0);
}
