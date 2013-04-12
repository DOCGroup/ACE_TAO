// -*- C++ -*-
// $Id$

#include "tao/GIOP_Message_Generator_Parser.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/GIOP_Utils.h"
#include "tao/debug.h"
#include "tao/CDR.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_GIOP_Message_Generator_Parser::~TAO_GIOP_Message_Generator_Parser (void)
{
}

int
TAO_GIOP_Message_Generator_Parser::parse_reply (
    TAO_InputCDR &stream,
    TAO_Pluggable_Reply_Params &params)
{
  // Read the request id
  if (!stream.read_ulong (params.request_id_))
    {
      if (TAO_debug_level)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) : TAO_GIOP_Message_Generator_Parser::parse_reply :")
                      ACE_TEXT ("extracting request id\n")));
        }
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD,
  // LOCATION_FORWARD_PERM
  CORBA::ULong rep_stat = 0;
  if (!stream.read_ulong (rep_stat))
    {
      if (TAO_debug_level > 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) : TAO_GIOP_Message_Generator_Parser::parse_reply, ")
                      ACE_TEXT ("extracting reply status\n")));
        }
      return -1;
    }

  params.reply_status (static_cast <GIOP::ReplyStatusType> (rep_stat));

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
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO (%P|%t|%N|%l):parse_locate_reply, ")
                    ACE_TEXT ("extracting request id\n")));

      return -1;
    }

  // and the locate reply status type.  status can be UNKNOWN_OBJECT,
  // OBJECT_HERE, OBJECT_FORWARD, OBJECT_FORWARD_PERM
  // LOC_SYSTEM_EXCEPTION, LOC_NEEDS_ADDRESSING_MODE

  // Please note here that we are NOT converting to the Pluggable
  // messaging layer exception as this is GIOP specific. Not many
  // messaging protocols have the locate_* messages.
  CORBA::ULong locate_reply_status;
  if (!cdr.read_ulong (locate_reply_status))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR ((LM_ERROR,
                    ACE_TEXT ("TAO N|(%P|%t|l) parse_locate_reply, ")
                    ACE_TEXT ("extracting locate reply status\n")));

      return -1;
    }
  params.locate_reply_status (static_cast <GIOP::LocateStatusType> (locate_reply_status));

  return 0;

}

bool
TAO_GIOP_Message_Generator_Parser::is_ready_for_bidirectional (void) const
{
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
