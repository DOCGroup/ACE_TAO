// -*- C++ -*-
//$Id$

#include "tao/GIOP_Message_Connectors.h"
#include "tao/Any.h"
#include "tao/debug.h"
#include "tao/Principal.h"
#include "tao/TAOC.h"
#include "tao/operation_details.h"
#include "tao/GIOP_Utils.h"
#include "tao/target_specification.h"
#include "tao/GIOPC.h"


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Connectors.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, GIOP_Message_Connectors, "$Id$")


int
TAO_GIOP_Message_Connectors::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Reply_Params &params)
{
  // @@ Bala: See how the message state is a per-protocol thing?
  //    Wouldn't it be better to have each protocol define its own
  //    message state and just use that type directly (i.e no
  //    message_state base class).  As far as i can see (but i may be
  //    wrong), only very tightly related messaging protocols and
  //    transport protocols can use the same message state (for
  //    example GIOP 1.x and GIOP Lite).  Furthermore, if i'm proven
  //    wrong and there is common functionality among different
  //    pluggable messaging protocols, we can simply re-factor at that
  //    point.
  //    IMHO the right approach is to factor out common functionality
  //    and interfaces 'a posteriori', otherwise we will define
  //    ackward interfaces that will have to be changed anyway!

  // @@ Carlos: I agree. But I see a problem. Other than the transport
  //    classes, we store state information in our Reply despatcers,
  //    Transport Mux Strategy etc. I would only love to have the
  //    Message States local to GIOP or say some other protocol. One
  //    example that spoils this idea is the Mux_Strategies that we
  //    have. I am ready to hear anything you suggest to get around
  //    this.


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

  // @@ Bala: More silly translations!

  // @@ Carlos: Please let me know what I can do to mask them from the
  //    user of this method

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
TAO_GIOP_Message_Connectors::
  parse_locate_reply (TAO_GIOP_Message_State &state,
                      TAO_Pluggable_Reply_Params &params)
{
  // Read the request id
  if (!state.cdr.read_ulong (params.request_id_))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t|%N|%l) : TAO_GIOP_Message_1_1::parse_reply, ")
                    ACE_TEXT ("extracting request id")));
      return -1;
    }

  // and the reply status type.  status can be NO_EXCEPTION,
  // SYSTEM_EXCEPTION, USER_EXCEPTION, LOCATION_FORWARD

  // Cannot handle LOCATION_FORWARD_PERM here

  // Please note here that we are NOT converting to the Pluggable
  // messaging layer exception as this is GIOP specific. Not many
  // messaging protocols have the locate_* messages.
  if (!state.cdr.read_ulong (params.reply_status_))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) : TAO_GIOP_Message_Connectors::parse_locate_reply, ")
                    ACE_TEXT ("extracting reply status\n")));
      return -1;
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
TAO_GIOP_Message_Connectors::process_client_message (TAO_Transport * /*transport*/, 
                                                     TAO_ORB_Core * /*orb_core*/,
                                                     TAO_InputCDR & /*input*/,
                                                     CORBA::Octet /*message_type*/)
{
   ACE_NOTSUP_RETURN (-1);
}


CORBA::Boolean
TAO_GIOP_Message_Connectors::write_reply_header (TAO_OutputCDR & /*cdr*/,
                                                 TAO_Pluggable_Reply_Params & /*params*/,
                                                 CORBA::Environment &
                                                 /*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
   ACE_NOTSUP_RETURN (0);
}


///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_10
//////////////////////////////////////////////////////////////////

CORBA::Boolean
TAO_GIOP_Message_Connector_10::
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
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    msg << CORBA::Any::from_octet (1);
  else 
    msg << CORBA::Any::from_octet (0);

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
                    ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
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
TAO_GIOP_Message_Connector_10::
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
                    ACE_TEXT ("(%N | %l) Unable to handle this request \n")));
      return 0;
    }

  return 1;
}


int
TAO_GIOP_Message_Connector_10::
  parse_reply (TAO_Message_State_Factory &mesg_state,
               TAO_Pluggable_Reply_Params &params)

{
  // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = ACE_dynamic_cast (TAO_GIOP_Message_State *,
                                                    &mesg_state);

  switch (state->message_type)
    {
    case TAO_GIOP_REQUEST:
      // In GIOP 1.0 and GIOP 1.1 this is an error,
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO (%P|%t) TAO_GIOP_Message_Connector_10::parse_reply: ") 
                         ACE_TEXT ("request.\n")),
                        -1);

    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      // Errors
    case TAO_GIOP_CLOSECONNECTION:
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) %N:%l parse_reply: ")
                           ACE_TEXT ("wrong message.\n")),
                          -1);
    case TAO_GIOP_LOCATEREPLY:
      if (this->parse_locate_reply (*state,
                                    params) == -1)
        return -1;
      break;
    case TAO_GIOP_REPLY:
      if ((state->cdr >> params.svc_ctx_) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                        ACE_TEXT ("extracting context\n")));
          return -1;
        }
      if (TAO_GIOP_Message_Connectors::parse_reply (*state,
                                                    params) == -1)
        return -1;
      break;
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
      break;
    }

  return 0;
}

CORBA::Octet
TAO_GIOP_Message_Connector_10:: major_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

CORBA::Octet
TAO_GIOP_Message_Connector_10:: minor_version (void)
{
  // Any harm in hardcoding??
  return 0;
}


///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_11
//////////////////////////////////////////////////////////////////

CORBA::Octet
TAO_GIOP_Message_Connector_11:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

///////////////////////////////////////////////////////////////////
// Methods for TAO_GIOP_Message_Connector_12
//////////////////////////////////////////////////////////////////

CORBA::Boolean
TAO_GIOP_Message_Connector_12::
  write_request_header (const TAO_Operation_Details &opdetails,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg)
{
  // First the request id
  msg << opdetails.request_id ();

  const CORBA::Octet response_flags = opdetails.response_flags ();
  
  // Here are the Octet values for different policies
  // '00000000' for SYNC_NONE
  // '00000001' for SYNC_WITH_TRANSPORT
  // '00000010' for SYNC_WITH_SERVER
  // '00000011' for SYNC_WITH_TARGET
  // '00000011' for regular two ways, but if they are invoked via a
  // DII with INV_NO_RESPONSE flag set then we need to send '00000001'
  //
  // We have not implemented the policy INV_NO_RESPONSE for DII.
  if (response_flags == TAO_TWOWAY_RESPONSE_FLAG)
    msg << CORBA::Any::from_octet (3);
  // Second the response flags
  // Sync scope - ignored by server if request is not oneway.
  else if (response_flags == CORBA::Octet (TAO::SYNC_NONE) ||
           response_flags == CORBA::Octet (TAO::SYNC_EAGER_BUFFERING) ||
           response_flags == CORBA::Octet (TAO::SYNC_DELAYED_BUFFERING))
    // No response required.
    msg << CORBA::Any::from_octet (0);

  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TRANSPORT))
    // Return after receiving message.
    msg << CORBA::Any::from_octet (1);

  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_SERVER))
    // Return before dispatching to the servant
    msg << CORBA::Any::from_octet (2);

  else if (response_flags == CORBA::Octet (TAO::SYNC_WITH_TARGET))
    // Return after dispatching servant.
    msg << CORBA::Any::from_octet (3);
  else
    // Until more flags are defined by the OMG.
    return 0;

  if (this->marshall_target_spec (spec,
                                  msg) == 0)
    return 0;
  
  // Write the operation name
  msg.write_string (opdetails.opname_len (), 
                    opdetails.opname ());
  
  // Write the service context list
  msg << opdetails.service_info ();

  // We need to align the pointer
  if (msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    return 0;

  return 1;
}


CORBA::Boolean
TAO_GIOP_Message_Connector_12::
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg)
{
  // Write the request id
  msg << request_id;
  
  // Write the target address
  if (this->marshall_target_spec (spec,
                                  msg) == 0)
    return 0;

  // We need to align the pointer
  if (msg.align_write_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR) == -1)
    return 0;

  // Return success
  return 1;
}

CORBA::Octet
TAO_GIOP_Message_Connector_12:: major_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 1;
}

CORBA::Octet
TAO_GIOP_Message_Connector_12:: minor_version (void)
{
  // Any harm in hardcoding??
  return (CORBA::Octet) 2;
}


CORBA::Boolean
TAO_GIOP_Message_Connector_12::
  marshall_target_spec (TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg)
{
  switch (spec.specifier ())
    {
    case TAO_Target_Specification::Key_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::KeyAddr;
        
        // Get the object key
        const TAO_ObjectKey *key = spec.object_key ();
        if (key)
          {
            // Marshall in the object key
            msg << *key;
          }
        else
          {
            if (TAO_debug_level)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    case TAO_Target_Specification::Profile_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::ProfileAddr;
        
        // Get the profile
        const IOP::TaggedProfile *pfile = spec.profile ();
        
        if (pfile)
          {
            // Marshall in the object key
            msg << *pfile;
          }
        else
          {
            if (TAO_debug_level)
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    case TAO_Target_Specification::Reference_Addr:
      {
        // As this is a union send in the discriminant first
        msg << GIOP::ReferenceAddr;
        
        // Get the IOR
        IOP::IOR *ior;
        CORBA::ULong index = spec.iop_ior (ior);
        
        if (ior)
          {
            // This is a struct IORAddressingInfo. So, marshall each
            // member of the struct one after another in the order
            // defined. 
            msg << index;
            msg << *ior;
          }
        else
          {
            if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
            return 0;
          }
        break;
      }
    default:
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%N |%l) Unable to handle this request \n")));
      return 0;
    }
    
  return 1;
}

int
TAO_GIOP_Message_Connector_12::
parse_reply (TAO_Message_State_Factory &mesg_state,
             TAO_Pluggable_Reply_Params &params)
{
  // Cast to the GIOP Message state
  TAO_GIOP_Message_State *state = ACE_dynamic_cast (TAO_GIOP_Message_State *,
                                                    &mesg_state);


  switch (state->message_type)
    {
    case TAO_GIOP_REQUEST:
      // We could get this in Bi_Dir GIOP
      // So, we take some action. 
      break;
    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      // Errors
    case TAO_GIOP_CLOSECONNECTION:
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("TAO (%P|%t) %N:%l parse_reply: ")
                           ACE_TEXT ("wrong message.\n")),
                          -1);
    case TAO_GIOP_LOCATEREPLY:
      if (this->parse_locate_reply (*state,
                                    params) == -1)
        return -1;
      break;
    case TAO_GIOP_REPLY:
      if (TAO_GIOP_Message_Connectors::parse_reply (*state,
                                                    params)
          == -1)
        return -1;
      if ((state->cdr >> params.svc_ctx_) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) parse_reply, ")
                        ACE_TEXT ("extracting context\n")));
          return -1;
        }
      // Rest of the stuff after the switch
      break;
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
      break;
    }
  
  // Align the read pointer on an 8-byte boundary
  state->cdr.align_read_ptr (TAO_GIOP_MESSAGE_ALIGN_PTR);
  return 0;
}
