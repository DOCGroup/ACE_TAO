//
// $Id$
//
#include "Hello.h"
#include "ace/ACE.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
    , mutex_ ()
    , latest_prime_ (0)
{
}

CORBA::ULong
Hello::next_prime (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    ace_mon,
                    this->mutex_,
                    this->latest_prime_);

  if (this->latest_prime_ > 10000)
    ACE_OS::abort ();

  for (CORBA::Long ind = 0;
       ind != 400;
       ++ind)
    {
      CORBA::Short bc = 0;

      while (bc == 0)
        {
          ++this->latest_prime_;
          if (ACE::is_prime (this->latest_prime_,
                             2,
                             this->latest_prime_ / 2) == 0)
            bc = 1;
        }
    }

  return this->latest_prime_;
}
