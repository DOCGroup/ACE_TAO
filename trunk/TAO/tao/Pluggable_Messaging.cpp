//$Id$

#include "tao/Pluggable_Messaging.h"
#include "tao/debug.h"
#include "tao/Pluggable.h"

#if !defined (__ACE_INLINE__)
# include "tao/Pluggable_Messaging.i"
#endif /* __ACE_INLINE__ */


ACE_RCSID(tao, Pluggable_Messaging, "$Id$")
//////////////////////////////////////////////////////////////////////////
// Methods for the Pluugable Messaging stuff
/////////////////////////////////////////////////////////////////////////

TAO_Pluggable_Messaging::~TAO_Pluggable_Messaging (void)
{
  //no-op
}

CORBA::Boolean
TAO_Pluggable_Messaging::write_reply_header (TAO_OutputCDR & /*cdr*/,
                                             TAO_Pluggable_Reply_Params & /*params*/,
                                             CORBA::Environment & /*ACE_TRY_ENV*/)
                       
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NOTSUP_RETURN (0);
}

int
TAO_Pluggable_Messaging:: transport_message (TAO_Transport *transport,
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
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) closing conn %d after fault %p\n"),
                    transport->handle (),
                    ACE_TEXT ("transport_message ()\n")));

      return -1;
    }

  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO: (%P|%t|%N|%l) send_message () \n")
                    ACE_TEXT ("EOF, closing conn %d\n"),
                    transport->handle()));
      return -1;
    }

  return 1;
}

TAO_Message_State_Factory::~TAO_Message_State_Factory (void)
{
  //no-op
}
