//$id$
#include "tao/GIOP_Message_Invocation.h"
#include "tao/GIOPC.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/Object_KeyC.h"
#include "tao/Principal.h"
#include "tao/GIOP_Utils.h"

//$Id$

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Invocation.i"
#endif /* __ACE_INLINE__ */

TAO_GIOP_Client_Message_1_1::TAO_GIOP_Client_Message_1_1 (void)
  :our_major_version_ (1),
   our_minor_version_ (1)
{
  //no-op
}


TAO_GIOP_Client_Message_1_1::~TAO_GIOP_Client_Message_1_1 (void)
{
  //no-op
}

int
TAO_GIOP_Client_Message_1_1::parse_header (TAO_GIOP_Message_State *state)
{
  if (TAO_GIOP_Utils::parse_giop_header (state,
                                         state->cdr) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N|%l) Error in parsing headers \n"),
                            -1);
        }
      return -1;
    }
     
  if (state->cdr.grow (this->get_header_len () +
                       state->message_size) == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP::handle_input, ACE_CDR::grow"));
      return -1;
    }

  // Growing the buffer may have reset the rd_ptr(), but we want
  // to leave it just after the GIOP header (that was parsed
  // already);
  state->cdr.skip_bytes (this->get_header_len ());
  
  return 1;
}

int
TAO_GIOP_Client_Message_1_1::parse_magic_bytes (TAO_InputCDR &input)
{
  // Grab the read pointer
  char *buf = input.rd_ptr ();
  
  // The values are hard-coded to support non-ASCII platforms.
  if (!(buf [0] == 0x47      // 'G'
        && buf [1] == 0x49   // 'I'
        && buf [2] == 0x4f   // 'O'
        && buf [3] == 0x50)) // 'P'
    {
      // Could be GIOPlite.. 
      //...
      
      // For the present...
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad header, magic word [%c%c%c%c]\n",
                    buf[0],
                    buf[1],
                    buf[2],
                    buf[3]));
      return -1;
    }
  
  if ((buf[TAO_GIOP_VERSION_MAJOR_OFFSET] != this->our_major_version_) ||
      (buf[TAO_GIOP_VERSION_MINOR_OFFSET] != this->our_minor_version_))
      return -1;

  return 1;
}

CORBA::Boolean
TAO_GIOP_Client_Message_1_1::write_request_header (const IOP::ServiceContextList& svc_ctx,
                                                   CORBA::ULong request_id,
                                                   CORBA::Octet response_flags,
                                                   TAO_Target_Specification &spec,
                                                   const char *opname,
                                                   TAO_OutputCDR &msg)
{
  // This i sepecific to GIOP 1.1. So put them here
  msg << svc_ctx;

  // Let us  call our parent class to check what he can do for
  // us. 
  TAO_GIOP_Client_Message_Factory::write_request_header (svc_ctx,
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
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "(%N | %l) Unable to handle this request \n"));
      return 0;
    }
  
  msg << opname;
  
  // The principal is not used. So send a null pointer
  static CORBA::Principal_ptr principal = 0;
  msg << principal;
  
  
  return 1;

}


CORBA::Boolean
TAO_GIOP_Client_Message_1_1::
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
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "(%N | %l) Unable to handle this request \n"));
      return 0;
    }
  
  return 1;
}


const size_t
TAO_GIOP_Client_Message_1_1::get_header_len (void)
{
  return TAO_GIOP_HEADER_LEN;
}

const size_t
TAO_GIOP_Client_Message_1_1::get_message_size_offset (void)
{
  return TAO_GIOP_MESSAGE_SIZE_OFFSET;
}


CORBA::Boolean 
TAO_GIOP_Client_Message_1_1::start_message (TAO_Pluggable_Message_Type t, 
                                            TAO_OutputCDR &msg)
{
  
  TAO_GIOP_Version version (this->our_major_version_,
                            this->our_minor_version_);
  
  TAO_GIOP_Message_Type type = TAO_GIOP_REQUEST;
  switch (t)
    {
    case (TAO_MESSAGE_REQUEST):
      type = TAO_GIOP_REQUEST;
      break;
    case (TAO_MESSAGE_REPLY):
      type = TAO_GIOP_REPLY;
      break;
    case (TAO_MESSAGE_CANCELREQUEST):
      type = TAO_GIOP_CANCELREQUEST;
      break;
    case (TAO_MESSAGE_LOCATEREQUEST):
      type = TAO_GIOP_LOCATEREQUEST;
      break;
    case (TAO_MESSAGE_LOCATEREPLY):
      type = TAO_GIOP_LOCATEREPLY;
      break;
    case (TAO_MESSAGE_CLOSECONNECTION):
      type = TAO_GIOP_CLOSECONNECTION;
      break;
    case (TAO_MESSAGE_MESSAGERROR):
      type = TAO_GIOP_MESSAGERROR;
      break;
    case (TAO_MESSAGE_FRAGMENT):
      type = TAO_GIOP_FRAGMENT;
      break;
    }
  
  return TAO_GIOP_Utils::start_message (version,
                                        type,
                                        msg);
}

int
TAO_GIOP_Client_Message_1_1::parse_reply (TAO_Transport * /*transport*/,
                                          TAO_Message_State_Factory &mesg_state,
                                          IOP::ServiceContextList& reply_ctx,
                                          CORBA::ULong &request_id,
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
                         "TAO (%P|%t) %N:%l TAO_GIOP::parse_reply: "
                         "request.\n"),
                        -1);

    case TAO_GIOP_CANCELREQUEST:
    case TAO_GIOP_LOCATEREQUEST:
      // Errors
    case TAO_GIOP_CLOSECONNECTION:
      // I am not sure why we are not handling this here.. by Bala
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                         "TAO (%P|%t) %N:%l TAO_GIOP::parse_reply: "
                         "wrong message.\n"),
                        -1);
        break;
    case TAO_GIOP_LOCATEREPLY:
      // Handle after the switch 
      break;
    case TAO_GIOP_REPLY:
      if ((state->cdr >> reply_ctx) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) TAO_GIOP_Client_Message_1_1::parse_reply, "
                        "extracting context\n"));
          return -1;
        }
      // Rest of the stuff after the switch
      break;
    case TAO_GIOP_FRAGMENT:
      // Never happens: why??
      break;
    }
  

  // Read the request id
  if (!state->cdr.read_ulong (request_id))
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
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
                    "TAO (%P|%t) : TAO_GIOP::parse_reply, "
                    "extracting reply status\n"));
      return -1;
    }
  
  return 0;
}
