// $Id$
#include "interface_1_i.h"

interface_1_i::interface_1_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
interface_1_i::operation_1 (void)
{
  return CORBA::string_dup ("Invoked operation one");
}

