#include "tao/GIOP_Message_Generator_Parser.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Utils.h"
#include "tao/debug.h"

#include "ace/Log_Msg.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Generator_Parser.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID (tao, 
           GIOP_Message_Gen_Parser, 
           "$Id$")

int
TAO_GIOP_Message_Generator_Parser::parse_reply (
    TAO_InputCDR &stream,
    TAO_Pluggable_Reply_Params &params)
{

  // Read the request id
  if (!stream.read_ulong (params.request_id_))
    {
      if (TAO_debug_level)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) : TAO_GIOP_Message_Generator_Parser::parse_reply, \n ")
                    ACE_TEXT ("extracting request id")));
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD

  // Cannot handle LOCATION_FORWARD_PERM here
  CORBA::ULong rep_stat = 0;
  if (!stream.read_ulong (rep_stat))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t) : TAO_GIOP_Message_Generator_Parser::parse_reply, \n ")
                    ACE_TEXT ("extracting reply status")));
      return -1;
    }


  // Pass the right Pluggable interface code to the transport layer
  switch (rep_stat)
    {
      // Request completed successfully
    case TAO_GIOP_NO_EXCEPTION:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
      break;

      // Request terminated with user exception
    case TAO_GIOP_USER_EXCEPTION:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION;
      break;
      // Request terminated with system exception
    case TAO_GIOP_SYSTEM_EXCEPTION:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION;
      break;
      // Reply is a location forward type
    case TAO_GIOP_LOCATION_FORWARD:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
      break;
      // Reply is a location forward perm type
      // @@For the time being the behaviour of the
      // LOCATION_FORWARD_PERM would be similar to the
      // LOCATION_FORWARD as there is a controversy surrounding the
      // usage of this in the OMG.
    case TAO_GIOP_LOCATION_FORWARD_PERM:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
      break;
      // Reply is a location forward type
    case TAO_GIOP_NEEDS_ADDRESSING_MODE:
      params.reply_status_ =
        TAO_PLUGGABLE_MESSAGE_NEEDS_ADDRESSING_MODE;
      break;
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%N|%l) Unknown reply status \n")));
    }

  return 0;
}


int
TAO_GIOP_Message_Generator_Parser::parse_locate_reply (
    TAO_InputCDR &cdr,
    TAO_Pluggable_Reply_Params &params)
{
  // Read the request id
  if (!cdr.read_ulong (params.request_id_))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t|%N|%l):parse_locate_reply, ")
                    ACE_TEXT ("extracting request id \n")));

      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD

  // Cannot handle LOCATION_FORWARD_PERM here

  // Please note here that we are NOT converting to the Pluggable
  // messaging layer exception as this is GIOP specific. Not many
  // messaging protocols have the locate_* messages.
  if (!cdr.read_ulong (params.reply_status_))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO N|(%P|%t|l) parse_locate_reply, ")
                    ACE_TEXT ("extracting reply  status\n")));

      return -1;
    }

  return 0;

}


int
TAO_GIOP_Message_Generator_Parser::is_ready_for_bidirectional (void)
{
  return 0;
}

void
TAO_GIOP_Message_Generator_Parser::marshal_reply_status (
    TAO_OutputCDR &output,
    TAO_Pluggable_Reply_Params_Base &reply
  )
{
  switch (reply.reply_status_)
    {
    case TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION:
      output.write_ulong (TAO_GIOP_NO_EXCEPTION);
      break;
    case TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD:
      output.write_ulong (TAO_GIOP_LOCATION_FORWARD);
      break;
    case TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION:
      output.write_ulong (TAO_GIOP_SYSTEM_EXCEPTION);
      break;
    case TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION:
      output.write_ulong (TAO_GIOP_USER_EXCEPTION);
      break;
    default:
      // Some other specific exception
      output.write_ulong (reply.reply_status_);
      break;
    }
}
