#include "Base_Server.h"

ACE_INLINE CORBA::ORB_ptr
Base_Server::orb () const
{
  return this->orb_.in ();
}
