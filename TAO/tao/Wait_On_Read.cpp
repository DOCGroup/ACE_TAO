// $Id$

#include "tao/Wait_On_Read.h"
#include "tao/Pluggable.h"

ACE_RCSID(tao, Wait_On_Read, "$Id$")

// Constructor.
TAO_Wait_On_Read::TAO_Wait_On_Read (TAO_Transport *transport)
  : TAO_Wait_Strategy (transport)
{
}

// Destructor.
TAO_Wait_On_Read::~TAO_Wait_On_Read (void)
{
}

// Wait on the read operation.
int
TAO_Wait_On_Read::wait (ACE_Time_Value * max_wait_time,
                        int &reply_received)
{
  reply_received = 0;
  while (reply_received != 1)
    {
      reply_received =
        this->transport_->read_process_message (max_wait_time, 1);
      if (reply_received == -1)
        return -1;
    }

  return 0;
}

// No-op.
int
TAO_Wait_On_Read::register_handler (void)
{
  return 0;
}

int
TAO_Wait_On_Read::non_blocking (void)
{
  return 0;
}
