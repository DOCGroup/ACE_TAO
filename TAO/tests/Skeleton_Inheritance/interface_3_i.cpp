// $Id$
#include "interface_3_i.h"

interface_3_i::interface_3_i (CORBA::ORB_ptr orb)
  : interface_1_i (orb)
  , interface_2_i (orb)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
interface_3_i::operation_3 (void)
{
  return CORBA::string_dup ("Invoked operation three");
}

void interface_3_i::shutdown (void)
{
  this->orb_->shutdown ();
}


