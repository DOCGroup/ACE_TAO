//$Id$
#include "tao/GIOP_Message_Connectors.h"
#include "tao/Any.h"
#include "tao/debug.h"
#include "tao/Principal.h"
#include "tao/TAOC.h"
#include "tao/operation_details.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Connectors.i"
#endif /* __ACE_INLINE__ */

int
TAO_GIOP_Message_Connectors::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Connector_Params &params)
{
  
  // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = ACE_dynamic_cast (TAO_GIOP_Message_State *,
                                                    &mesg_state);

  // Read the request id
  if (!state->cdr.read_ulong (params.request_id_))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t|%N|%l) : TAO_GIOP_Message_1_1::parse_reply, "
                    "extracting request id"));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD

  // Cannot handle LOCATION_FORWARD_PERM here
  CORBA::ULong rep_stat = 0;
  if (!state->cdr.read_ulong (rep_stat))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t|%N|%l) : TAO_GIOP_Message_1_1::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }
  
  // Pass the right Pluggable interface code to the transport layer
  switch (rep_stat)
    {
      // Request completed successfully
    case TAO_GIOP_NO_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_NO_EXCEPTION;
      break;
      
      // Request terminated with user exception
    case TAO_GIOP_USER_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_USER_EXCEPTION;
      break;
      // Request terminated with system exception        
    case TAO_GIOP_SYSTEM_EXCEPTION:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_SYSTEM_EXCEPTION;
      break;
      // Reply is a location forward type
    case TAO_GIOP_LOCATION_FORWARD:
      params.reply_status_ = TAO_PLUGGABLE_MESSAGE_LOCATION_FORWARD;
      break;
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%N|%l) Unknown reply status \n")));
    }
  
  return 0;
}


int
TAO_GIOP_Message_Connectors::validate_version (TAO_GIOP_Message_State *state)
  
{
  // Grab the read pointer
  char *buf = state->cdr.rd_ptr ();

  if ((buf[TAO_GIOP_VERSION_MAJOR_OFFSET] != this->major_version ()) ||
       (buf[TAO_GIOP_VERSION_MINOR_OFFSET] != this->minor_version ()))
    return -1;
  
  state->giop_version.major = buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  state->giop_version.minor = buf[TAO_GIOP_VERSION_MINOR_OFFSET];
  
  return 0;
}

int
TAO_GIOP_Message_Connectors::
process_connector_messages (TAO_Transport * /*transport*/, 
                            TAO_ORB_Core * /*orb_core*/,
                            TAO_InputCDR & /*input*/,
                            CORBA::Octet /*message_type*/)
{
   ACE_NOTSUP_RETURN (-1);
}


///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_11
//////////////////////////////////////////////////////////////////

CORBA::Boolean
TAO_GIOP_Message_Connector_11::
  write_request_header (const TAO_Operation_Details &opdetails,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg)
{
  // This is sepecific to GIOP 1.1. So put them here
  msg << opdetails.service_info ();

  // First the request id
  msg << opdetails.request_id ();

  const CORBA::Octet response_flags = opdetails.response_flags ();

   // @@ (JP) Temporary hack until all of GIOP 1.2 is implemented.
  if (response_flags == 131)
    msg << CORBA::Any::from_octet (1);

  // Second the response flags
  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TRANSPORT) ||
           response_flags == CORBA::Octet (TAO::SYNC_NONE) ||
           response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING) ||
           response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    // No response required.
    msg << CORBA::Any::from_octet (0);
  
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_SERVER))
    // Return before dispatching servant.  We're also setting the high
    // bit here. This is a temporary fix until the rest of GIOP 1.2 is
    // implemented in TAO.
    msg << CORBA::Any::from_octet (129);
  
  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TARGET))
    // Return after dispatching servant.
    msg << CORBA::Any::from_octet (3);

  else
    // Until more flags are defined by the OMG.
    return 0;

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO_ObjectKey *key = spec.object_key ();
  if (key)
    {
      // Put in the object key
      msg << *key;
    }
  else
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }

  msg.write_string (opdetails.opname_len (), 
                    opdetails.opname ());
  
  // Last element of request header is the principal; no portable way
  // to get it, we just pass empty principal (convention: indicates
  // "anybody").  Steps upward in security include passing an
  // unverified user ID, and then verifying the message (i.e. a dummy
  // service context entry is set up to hold a digital signature for
  // this message, then patched shortly before it's sent).
  static CORBA::Principal_ptr principal = 0;
  msg << principal;


  return 1;

}




CORBA::Boolean
TAO_GIOP_Message_Connector_11::
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg)
{
  msg << request_id;

  // In this case we cannot recognise anything other than the Object
  // key as the address disposition variable. But we do a sanity check
  // anyway.
  const TAO_ObjectKey *key = spec.object_key ();
  if (key)
    {
      // Everything is fine
      msg << *key;
    }
  else
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%N | %l) Unable to handle this request \n")));
      return 0;
    }
 
  return 1;
}


int
TAO_GIOP_Message_Connector_11::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Connector_Params &params)
       
{
  // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = ACE_dynamic_cast (TAO_GIOP_Message_State *,
                                                    &mesg_state);

  switch (state->message_type)
    {
    case TAO_GIOP_REQUEST:
      // In GIOP 1.0 and GIOP 1.1 this is an error,
      ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l TAO_GIOP_Message_1_1::parse_reply: "
                         "request.\n"),
                        -1);

    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      // Errors
    case TAO_GIOP_CLOSECONNECTION:
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("TAO (%P|%t) %N:%l parse_reply: ") 
                           ASYS_TEXT ("wrong message.\n")),
                          -1);
    case TAO_GIOP_LOCATEREPLY:
      // Handle after the switch
      break;
    case TAO_GIOP_REPLY:
      if ((state->cdr >> params.svc_ctx_) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("TAO (%P|%t) parse_reply, ")
                        ASYS_TEXT ("extracting context\n")));
          return -1;
        }
      // Rest of the stuff after the switch
      break;
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
      break;
    }
  
  if (TAO_GIOP_Message_Connectors::parse_reply (*state,
                                                params) 
      == -1)
    return -1;

  return 0;
}

CORBA::Octet
TAO_GIOP_Message_Connector_11:: major_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

CORBA::Octet
TAO_GIOP_Message_Connector_11:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}


///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_10
//////////////////////////////////////////////////////////////////

CORBA::Octet
TAO_GIOP_Message_Connector_10:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 0;
}
