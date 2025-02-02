#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::method(Test::Hello_out r)
{
  Test::Hello_var v;
  r = v;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
