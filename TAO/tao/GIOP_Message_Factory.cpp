//$Id$
// @(#)giop.cpp 1.10 95/09/21
// Copyright 1994-1995 by Sun Microsystems Inc.
// All Rights Reserved
//
// GIOP:        Utility routines for sending, receiving GIOP messages
//
// Note that the Internet IOP is just the TCP-specific mapping of the
// General IOP.  Areas where other protocols may map differently
// include use of record streams (TCP has none), orderly disconnect
// (TCP has it), endpoint addressing (TCP uses host + port), security
// (Internet security should be leveraged by IIOP) and more.
//
// NOTE: There are a few places where this code knows that it's really
// talking IIOP instead of GIOP.  No rush to fix this so long as we
// are really not running atop multiple connection protocols.
//
// THREADING NOTE: currently, the connection manager eliminates tricky
// threading issues by providing this code with the same programming
// model both in threaded and unthreaded environments.  Since the GIOP
// APIs were all designed to be reentrant, this makes threading rather
// simple!
//
// That threading model is that the thread making (or handling) a call
// is given exclusive access to a connection for the duration of a
// call, so that no multiplexing or demultiplexing is needed.  That
// is, locking is at the "connection level" rather than "message
// level".

// The down side of this simple threading model is that utilization of
// system resources (mostly connections, but to some extent network
// I/O) in some kinds of environments can be inefficient.  However,
// simpler threading models are much easier to get properly debugged,
// and often perform better.  Also, such environments haven't been
// seen to be any kind of problem; the model can be changed later if
// needed, it's just an internal implementation detail.  Any portable
// ORB client is not allowed to rely on semantic implications of such
// a model.
//
// @@ there is lots of unverified I/O here.  In all cases, if an
// error is detected when marshaling or unmarshaling, it should be
// reported.

#include "tao/GIOP_Message_Factory.h"
#include "tao/GIOP_Utils.h"
#include "tao/Any.h"
#include "tao/ORB_Core.h"


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Factory.i"
#endif /* __ACE_INLINE__ */



//
// Client Side Message Factory Methods
// 

TAO_GIOP_Client_Message_Factory::TAO_GIOP_Client_Message_Factory (void)
{
}

TAO_GIOP_Client_Message_Factory::~TAO_GIOP_Client_Message_Factory (void)
{
}

int
TAO_GIOP_Client_Message_Factory::handle_input (TAO_Transport *transport,
                                               TAO_ORB_Core * /*orb_core*/,
                                               TAO_Message_State_Factory &mesg_state,
                                               ACE_Time_Value *max_wait_time)
{
  TAO_GIOP_Message_State *state = 
    ACE_dynamic_cast (TAO_GIOP_Message_State *,
                      &mesg_state);

  if (state->header_received () == 0)
    {
      if (TAO_GIOP_Utils::read_bytes_input (transport,
                                            state->cdr,
                                            TAO_GIOP_HEADER_LEN,
                                            max_wait_time) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                        "TAO_GIOP_Client_Message_Factory::handle_input"));
          return -1;
        }
      
      
      if (this->parse_magic_bytes (state->cdr) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                        "TAO_GIOP_Client_Message_Factory::handle_input, parse_bytes")); 
          return -1;
        }
      
      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->parse_header (state) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                          "TAO_GIOP_Client_Message_Factory::handle_input"));
          return -1;
        }
    }
  
  size_t missing_data =
    state->message_size - state->current_offset;
  

  ssize_t n = 
    TAO_GIOP_Utils::read_buffer (transport,
                                 state->cdr.rd_ptr () + state->current_offset,
                                 missing_data,
                                 max_wait_time);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP_Client_Message_Factory::handle_input, read_buffer[1]"));
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP_Client_Message::handle_input, read_buffer[2]"));
      return -1;
    }
  
  state->current_offset += n;
  
  if (state->current_offset == state->message_size)
    {
      if (TAO_debug_level >= 4)
        {
          size_t header_len = TAO_GIOP_HEADER_LEN;

          // Need to include GIOPlite too.
         
          char *buf = state->cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state->cdr.length () + header_len;
          TAO_GIOP_Utils::dump_msg ("recv",
                                    ACE_reinterpret_cast (u_char *,
                                                          buf),
                                    msg_len);
        }
    }

  return state->is_complete ();
}


CORBA::Boolean
TAO_GIOP_Client_Message_Factory::write_request_header (const IOP::ServiceContextList& /*svc_ctx*/,
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

  // Second the response flags
  switch (response_flags)
    {
      // We have to use magic numbers as the actual variables are
      // declared as const short. They cannot be used in switch case
      // statements. Probably what we can do is to add them as an
      // definitions in this class or in the parent and then use the
      // definitions. Hmm. Good idea.. But we will live with this for
      // the time being. 
    case 0: // SYNC_NONE
    case 1: // SYNC_WITH_TRANSPORT
    case 4: // This one corresponds to the TAO extension SYNC_FLUSH.
      // No response required.
      msg << CORBA::Any::from_octet (0);
        break;
    case 2: // SYNC_WITH_SERVER
      // Return before dispatching servant.
      msg << CORBA::Any::from_octet (1);
      break;
    case 3: // SYNC_WITH_TARGET
      // Return after dispatching servant.
      msg << CORBA::Any::from_octet (3);
      break;
      // Some cases for the DII are missing here. We can add that once
      // our IDL compiler starts supporting those stuff. This is
      // specific to GIOP 1.2. So some of the services would start
      // using this at some point of time and we will have them here
      // naturally out of a need.
    default:
      return 0;
    }
  
  return 1;
}



int                                                
TAO_GIOP_Client_Message_Factory::send_message (TAO_Transport *transport,
                                               TAO_OutputCDR &stream,
                                               ACE_Time_Value *max_wait_time,
                                               TAO_Stub *stub)
{
  // Get the header length
  size_t header_len = this->get_header_len ();

  // Get the message size offset
  size_t offset = this->get_message_size_offset ();
  
  return TAO_GIOP_Utils::send_message (transport,
                                       stream,
                                       header_len,
                                       offset,
                                       max_wait_time,
                                       stub);
}



//
// Server Side Message Factory Methods
// 


TAO_GIOP_Message_Acceptor::TAO_GIOP_Message_Acceptor (void)
{
  // Initialised...
  this->accept_states_ =  &(this->available_states_.giop_1_1_);
  
}

TAO_GIOP_Message_Acceptor::~TAO_GIOP_Message_Acceptor (void)
{
  //no-op
}
  
int
TAO_GIOP_Message_Acceptor::handle_input (TAO_Transport *transport,
                                         TAO_ORB_Core * /*orb_core*/,
                                         TAO_Message_State_Factory &mesg_state,
                                         ACE_Time_Value *max_wait_time)
{
  TAO_GIOP_Message_State *state = 
    ACE_dynamic_cast (TAO_GIOP_Message_State *,
                      &mesg_state);

  if (state->header_received () == 0)
    {
      if (TAO_GIOP_Utils::read_bytes_input (transport,
                                            state->cdr,
                                            TAO_GIOP_HEADER_LEN,
                                            max_wait_time) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                        "TAO_GIOP_Message_Acceptor::handle_input"));
          return -1;
        }
      
      if (this->parse_magic_bytes (state->cdr, 
                                   state) == -1)
        {
          this->accept_states_->send_error (transport);
          
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                        "TAO_GIOP_Message_Acceptor::handle_input, parse_bytes")); 
          return -1;
        }
      
      // Read the rest of the stuff. That should be read by the
      // corresponding states
      if (this->accept_states_->parse_header (state) == -1)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "TAO (%P|%t) - %p\n"
                          "TAO_GIOP_Message_Acceptor::handle_input"));
          return -1;
        }
    }
  
  size_t missing_data =
    state->message_size - state->current_offset;
  
  ssize_t n = 
    TAO_GIOP_Utils::read_buffer (transport,
                                 state->cdr.rd_ptr () + state->current_offset,
                                 missing_data,
                                 max_wait_time);
  if (n == -1)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP_Message_Acceptor::handle_input, read_buffer[1]"));
      return -1;
    }
  else if (n == 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) - %p\n",
                    "TAO_GIOP_Message_Acceptor::handle_input, read_buffer[2]"));
      return -1;
    }
  
  state->current_offset += n;
  
  if (state->current_offset == state->message_size)
    {
      if (TAO_debug_level >= 4)
        {
          size_t header_len = TAO_GIOP_HEADER_LEN;

          // Need to include GIOPlite too.
         
          char *buf = state->cdr.rd_ptr ();
          buf -= header_len;
          size_t msg_len = state->cdr.length () + header_len;
          TAO_GIOP_Utils::dump_msg ("recv",
                                    ACE_reinterpret_cast (u_char *,
                                                          buf),
                                    msg_len);
        }
    }

  return state->is_complete ();
}


int
TAO_GIOP_Message_Acceptor::
process_connector_messages (TAO_Transport *transport, 
                            TAO_ORB_Core *orb_core,
                            TAO_InputCDR &input,
                            CORBA::Octet message_type)
{
  char repbuf[ACE_CDR::DEFAULT_BUFSIZE];
  
#if defined(ACE_HAS_PURIFY)
  (void) ACE_OS::memset (repbuf,
                         '\0',
                         sizeof repbuf);
#endif /* ACE_HAS_PURIFY */
  
  TAO_OutputCDR output (repbuf,
                        sizeof repbuf,
                        TAO_ENCAP_BYTE_ORDER,
                        orb_core->output_cdr_buffer_allocator (),
                        orb_core->output_cdr_dblock_allocator (),
                        orb_core->orb_params ()->cdr_memcpy_tradeoff (),
                        orb_core->to_iso8859 (),
                        orb_core->to_unicode ());

  switch (message_type)
    {
    case TAO_GIOP_REQUEST:
      // Should be taken care by the state specific invocations. They
      // could raise an exception or write things in the output CDR
      // stream 
      this->accept_states_->process_connector_request (transport,
                                                       orb_core,
                                                       input,
                                                       output);
      break;
    case TAO_GIOP_LOCATEREQUEST:
      this->accept_states_->process_connector_locate (transport,
                                                      orb_core,
                                                      input,
                                                      output);
      break;
    case TAO_GIOP_MESSAGERROR:
    case TAO_GIOP_REPLY:
    case TAO_GIOP_LOCATEREPLY:
    case TAO_GIOP_CLOSECONNECTION:
    default:
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) Illegal message received by server\n"));
      return this->accept_states_->send_error (transport);
      break;
    }

  return 0;
}

int 
TAO_GIOP_Message_Acceptor::parse_magic_bytes (TAO_InputCDR &input,
                                              TAO_GIOP_Message_State *state)
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
  
  state->giop_version.major = buf[TAO_GIOP_VERSION_MAJOR_OFFSET];
  state->giop_version.minor = buf[TAO_GIOP_VERSION_MINOR_OFFSET];

  // An extra variable?? Huh?? Can be removed??
  CORBA::Octet major = state->giop_version.major;
  CORBA::Octet minor = state->giop_version.minor;
  
  if ((this->available_states_.check_major (major) == -1) ||
      (this->available_states_.check_minor (minor) == -1)) 
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO (%P|%t) bad version <%d.%d>\n",
                    major, minor));
      return -1;
    }
  
  // Set the appropriate state 
  if (this->set_state (state) == -1)
    {
      if (TAO_debug_level > 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%P|%t|%N|%l) Failure <error> in set_state ()\n"),
                            -1);
        }
    }

  return 0;
}


int
TAO_GIOP_Message_Acceptor::set_state (TAO_GIOP_Message_State * /*state*/)
                                      
{
  // Logic for selecting from a registry set should be here. Let me go
  // first with  normal stuff.
  this->accept_states_ = &(this->available_states_.giop_1_1_);

  return 1;
}

