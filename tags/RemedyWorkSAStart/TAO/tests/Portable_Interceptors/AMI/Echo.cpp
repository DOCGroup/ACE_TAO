/**
 * @file Echo.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "Echo.h"

Echo::Echo(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

char *
Echo::echo_operation(char const * the_input)
{
  return CORBA::string_dup(the_input);
}

void
Echo::shutdown(void)
{
  this->orb_->shutdown(0);
}
