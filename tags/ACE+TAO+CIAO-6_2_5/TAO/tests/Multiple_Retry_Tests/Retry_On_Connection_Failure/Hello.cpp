// $Id$
#include "Hello.h"
#include <iostream>

Hello::Hello (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

char *
Hello::get_string (void)
{
  return CORBA::string_dup ("Hello there!");
}

void
Hello::set_string (const char * s)
{
  std::cout << "Call to Hello::set_string (" << s << ")" << std::endl;
}

void
Hello::shutdown (void)
{
  this->orb_->shutdown (0);
}
