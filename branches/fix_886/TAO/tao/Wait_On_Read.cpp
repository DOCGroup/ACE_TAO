// $Id$

#include "tao/Wait_On_Read.h"
#include "tao/Transport.h"
#include "tao/Resume_Handle.h"
#include "tao/Synch_Reply_Dispatcher.h"

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
                        TAO_Synch_Reply_Dispatcher &rd)
{
  rd.reply_received () = 0;

  // Do the same sort of looping that is done in other wait
  // strategies.
  int retval = 0;
  TAO_Resume_Handle rh;
  while (1)
    {
      retval =
        this->transport_->handle_input_i (rh,
                                          max_wait_time,
                                          1);

      // If we got our reply, no need to run the loop any
      // further.
      if (rd.reply_received ())
        break;

      // @@ We are not checking for timeouts here...

      // If we got an error just break
      if (retval == -1)
        break;
    }

  if (rd.reply_received () == -1 || retval == -1)
    {
      this->transport_->close_connection ();
    }

  return (rd.reply_received () == 1 ? 0 : rd.reply_received ());
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
