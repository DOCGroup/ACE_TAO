//$Id$
#include "tao/GIOP_Message_Connectors.h"
#include "tao/Any.h"
#include "tao/debug.h"
#include "tao/Principal.h"
#include "tao/TAOC.h"

#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Connectors.i"
#endif /* __ACE_INLINE__ */

CORBA::Boolean
TAO_GIOP_Message_Connectors::
  write_request_header (const IOP::ServiceContextList& /*svc_ctx*/,
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification & /*spec*/,
                        const char * /*opname*/,
                        TAO_OutputCDR &msg)
{
  // Adding only stuff that are common to all versions of GIOP.
  // @@ Note: If at any stage we feel that this amount of granularity
  // for factorisation is not important we can dispense with it
  // anyway.

  // First the request id
  msg << request_id;

   // @@ (JP) Temporary hack until all of GIOP 1.2 is implemented.
  if (response_flags == 131)
    msg << CORBA::Any::from_octet (1);

  // Second the response flags
  // Sync scope - ignored by server if request is not oneway.
  if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TRANSPORT) ||
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

  return 1;
}

int
TAO_GIOP_Message_Connectors::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Connector_Params &params,
               CORBA::ULong &reply_status)
{
  
    // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = ACE_dynamic_cast (TAO_GIOP_Message_State *,
                                                    &mesg_state);

  // Read the request id
  if (!state->cdr.read_ulong (params.request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t|%N|%l) : TAO_GIOP_Message_1_1::parse_reply, "
                    "extracting request id"));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD
  // CAnnot handle LOCATION_FORWARD_PERM here
  if (!state->cdr.read_ulong (reply_status))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t|%N|%l) : TAO_GIOP_Message_1_1::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }

  return 0;
}


int
TAO_GIOP_Message_Connectors::validate_version (TAO_GIOP_Message_State *state)
  
{
  // Grab the read pointer
  char *buf = state->cdr.rd_ptr ();

  if ((buf[this->major_version_offset ()] != this->major_version ()) ||
       (buf[this->minor_version_offset ()] != this->minor_version ()))
    return -1;
  
  state->giop_version.major = buf[this->major_version_offset ()];
  state->giop_version.minor = buf[this->minor_version_offset ()];
  
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
  write_request_header (const IOP::ServiceContextList& svc_ctx, 
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification &spec,
                        const char *opname,
                        TAO_OutputCDR &msg)
{
  // This is sepecific to GIOP 1.1. So put them here
  msg << svc_ctx;

  // Let us  call our parent class to check what he can do for
  // us.
  TAO_GIOP_Message_Connectors::write_request_header (svc_ctx,
                                                     request_id,
                                                     response_flags,
                                                     spec,
                                                     opname,
                                                     msg);

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

  msg << opname;

  // The principal is not used. So send a null pointer
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
               TAO_Pluggable_Connector_Params &params,
               CORBA::ULong &reply_status)
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
        break;
    case TAO_GIOP_LOCATEREPLY:
      // Handle after the switch
      break;
    case TAO_GIOP_REPLY:
      if ((state->cdr >> params.svc_ctx) == 0)
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
                                                params,
                                                reply_status) == -1)
    return -1;

  return 0;
}
