#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr const orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Any *
Hello::get_any (CORBA::Any const & the_any)
{
  CORBA::Any *retval {};

  // The caller takes ownership of this newly allocated Any.
  ACE_NEW_THROW_EX (retval,
                    CORBA::Any (the_any),
                    CORBA::NO_MEMORY ());

  return retval;
}


void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
