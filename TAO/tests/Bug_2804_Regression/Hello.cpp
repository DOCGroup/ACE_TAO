#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

CORBA::Any *
Hello::get_any (CORBA::Any const & the_any)
{
  CORBA::Any *retval {};

  ACE_NEW_THROW_EX (retval,
                    CORBA::Any (the_any),
                    CORBA::NO_MEMORY ());

  // The operation return value transfers ownership of this allocated Any to TAO.
  return retval;
}


void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
