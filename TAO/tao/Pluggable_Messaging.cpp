//$Id$

#include "tao/Pluggable_Messaging.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */



//////////////////////////////////////////////////////////////////////////
// Methods for the Pluugable Messaging stuff
/////////////////////////////////////////////////////////////////////////

TAO_Pluggable_Messaging_Interface::~TAO_Pluggable_Messaging_Interface (void)
{
  //no-op
}

int
TAO_Pluggable_Messaging_Interface::
  transport_message (TAO_Transport *transport,
                     TAO_OutputCDR &stream,
                     int two_way,
                     TAO_Stub *stub,
                     ACE_Time_Value *max_wait_time)
{
  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = transport->send (stub,
                               two_way,
                               stream.begin (),
                               max_wait_time);

  if (n == -1)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO: (%P|%t|%N|%l) closing conn %d after fault %p\n"),
                    transport->handle (),
                    ASYS_TEXT ("GIOP_Message_Base::send_message ()")));

      return -1;
    }
  
  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO: (%P|%t|%N|%l) send_message () ")
                    ASYS_TEXT ("EOF, closing conn %d\n"),
                    transport->handle()));
      return -1;
    }

  return 1;
}

TAO_Message_State_Factory::~TAO_Message_State_Factory (void)
{
  //no-op
}
