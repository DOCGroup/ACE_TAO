// $Id$
#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  static int our_count = 0;

  if (our_count == 0)
    {
      ++our_count;
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
