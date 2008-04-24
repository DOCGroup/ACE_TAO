//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

int count = 0;

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  if (count == 0)
    {
      ++count;
      throw ::CORBA::TRANSIENT (0,
              CORBA::COMPLETED_MAYBE);
    }

  return CORBA::string_dup ("You shouldn't ever see this message");
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
