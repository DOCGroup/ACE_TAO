#include "Simple.h"
#include "ace/OS_NS_unistd.h"
#include <array>
#include <cstddef>

static std::array<char const *, 3> const locations {
  "MyLocation 1", "MyLocation 2", "MyLocation 3"
};

Simple::Simple (int server_id, int pong_delay)
  : server_id_ (server_id),
    pong_delay_ (pong_delay)
{
}

/// Default _non_existent: always returns false.
CORBA::Boolean
Simple::_non_existent ()
{
  if (this->pong_delay_ > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T Simple::_non_existent server %d sleep %d seconds ... \n",
      this->server_id_, this->pong_delay_));
  for (int i {}; i < this->pong_delay_; ++i)
    ACE_OS::sleep (1);
  return false;
}

char *
Simple::get_string ()
{
  if (this->server_id_ < 1
      || static_cast<size_t> (this->server_id_) > locations.size ())
    return {};
  return CORBA::string_dup (locations[static_cast<size_t> (this->server_id_ - 1)]);
}

void
Simple::exit ()
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t)%T - Simple::exit server %d exit\n",
              this->server_id_));
  ACE_OS::exit ();
}

