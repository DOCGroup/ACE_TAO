// $Id$
#include "tao/Invocation_Utils.h"

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     ncalls_ (0),
     raise_exception_ (TAO::FOE_NON)
{
}


ACE_INLINE
int Simple_Server_i::ncalls () const
{
  return this->ncalls_;
}


