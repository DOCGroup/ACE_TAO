#include "Hello.h"

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (
        ::CORBA::Long ,
        char const *inputa,
        ::CORBA::Long ,
        char const * )
{
  return CORBA::string_dup (inputa);
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
