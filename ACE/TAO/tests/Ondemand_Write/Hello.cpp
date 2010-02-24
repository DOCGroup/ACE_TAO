//
// $Id$
//
#include "Hello.h"

ACE_RCSID(Hello, Hello, "$Id$")

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (
        ::CORBA::Long ,
        const char * inputa,
        ::CORBA::Long ,
        const char * )
{
  return CORBA::string_dup (inputa);
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
