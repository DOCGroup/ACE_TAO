// $Id$

#include "client_i.h"

client_i::client_i (int quiet,
                    server_ptr s)
  : quiet_ (quiet),
    server_ (server::_duplicate (s))
{
}

void
client_i::ping (CORBA::UShort time_to_live)
{
  if (!this->quiet_)
    ACE_DEBUG ((LM_DEBUG,
                "(%t) client_i::ping -> time to live = %d\n",
                time_to_live));

  --time_to_live;

  if (time_to_live > 0)
    {
      this->server_->ping (time_to_live);
    }
}
