#include "Hello.h"
#include "ace/ACE.h"

CORBA::ULong
Hello::next_prime ()
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
      bool is_prime = false;

      while (!is_prime)
        {
          ++this->latest_prime_;
          if (ACE::is_prime (this->latest_prime_,
                             2,
                             this->latest_prime_ / 2) == 0)
            is_prime = true;
        }
    }

  return this->latest_prime_;
}
