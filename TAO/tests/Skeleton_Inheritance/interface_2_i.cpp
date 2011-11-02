// $Id$
#include "interface_2_i.h"

interface_2_i::interface_2_i (CORBA::ORB_ptr orb)
  : interface_1_i(orb)
  , orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
interface_2_i::operation_2 (void)
{
  return CORBA::string_dup ("Invoked operation two");
}

