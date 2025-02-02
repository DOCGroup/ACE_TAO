#include "Base_Server.h"

ACE_INLINE CORBA::ORB_ptr
Base_Server::orb ()
{
  return this->orb_.in();
}
