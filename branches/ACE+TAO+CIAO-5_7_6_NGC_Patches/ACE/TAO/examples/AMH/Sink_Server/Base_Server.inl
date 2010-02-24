// $Id$

#include "Base_Server.h"

ACE_INLINE CORBA::ORB_ptr
Base_Server::orb (void)
{
  return this->orb_.in();
}
