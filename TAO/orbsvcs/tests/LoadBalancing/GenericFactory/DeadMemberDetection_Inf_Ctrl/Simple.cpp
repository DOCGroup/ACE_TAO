// $Id$

#include "Simple.h"
#include "ace/OS.h"

extern int pong_delay;

const char *location1 = "MyLocation 1";
const char *location2 = "MyLocation 2";
const char *location3 = "MyLocation 3";

Simple::Simple (int server_id)
: server_id_ (server_id)
{
}

/// Default _non_existent: always returns false.
CORBA::Boolean
Simple::_non_existent (void)
{
  if (pong_delay > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T Simple::_non_existent server %d sleep %d seconds ... \n",
      server_id_, pong_delay));
  int i = 0;
  while (i < pong_delay)
  {
    ++i;
    ACE_OS::sleep (1);
  }
  return false;
}

char *
Simple::get_string (void)
{
  switch (this->server_id_)
  {
    case 1:
      return CORBA::string_dup (location1);
      break;
    case 2:
      return CORBA::string_dup (location2);
      break;
    case 3:
      return CORBA::string_dup (location3);
      break;
    default:
      return 0;
      break;
  }
}

void
Simple::exit (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Simple::exit  server %d exit \n",
                      this->server_id_));
  ACE_OS::exit ();
}

