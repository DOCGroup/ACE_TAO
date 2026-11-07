#include "tao/Invocation_Utils.h"

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, int raise_exception)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    raise_exception_ (raise_exception)
{
}

ACE_INLINE
int Simple_Server_i::ncalls () const
{
  return this->ncalls_;
}


