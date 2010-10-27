//
// $Id$
//
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
Hello::op (
        ::Test::Fls_out fstruct,
        ::Test::Vls_out vstruct
      )
{
  Test::Fls a = {5, 1.0};
  fstruct = a;
  vstruct = new Test::Vls;
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
