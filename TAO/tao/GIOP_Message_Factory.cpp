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
#include "tao/Any.h"


#if !defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Factory.i"
#endif /* __ACE_INLINE__ */

TAO_GIOP_Message_Factory::TAO_GIOP_Message_Factory (void)
{
}

TAO_GIOP_Message_Factory::~TAO_GIOP_Message_Factory (void)
{
}

/*CORBA::Boolean
TAO_GIOP_Message_Factory::start_message (const TAO_GIOP_Version &version,
                                        TAO_GIOP_Message_Factory::Message_Type t,
                                        TAO_OutputCDR &msg)
{
  msg.reset ();

   static CORBA::Octet magic[] =
  {
    // The following works on non-ASCII platforms, such as MVS (which
    // uses EBCDIC).
    0x47, // 'G'
    0x49, // 'I'
    0x4f, // 'O'
    0x50, // 'P'
  };

  static int magic_size = sizeof (magic)/sizeof (magic[0]);
  msg.write_octet_array (magic, magic_size);
  msg.write_octet (version.major);
  msg.write_octet (version.minor);
  msg.write_octet (TAO_ENCAP_BYTE_ORDER);
  msg.write_octet ((CORBA::Octet) type);

  // Write a dummy <size> later it is set to the right value...
  CORBA::ULong size = 0;
  msg.write_ulong (size);

  return 1;
}
*/

CORBA::Boolean
TAO_GIOP_Message_Factory::write_request_header (const IOP::ServiceContextList& /*svc_ctx*/,
                                                CORBA::ULong request_id,
                                                CORBA::Octet response_flags,
                                                TAO_Stub */*stub*/,
                                                const CORBA::Short address_disposition,
                                                const char */*opname*/,
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
      // Until more flags are defined by the OMG.
      return 0;
    }
  
  return 1;
}



                                                
TAO_GIOP_Message_Factory::send_message (TAO_Transport *transport,
                                        TAO_OutputCDR &stream,
                                        ACE_Time_Value *max_wait_time = 0,
                                        TAO_Stub *stub = 0)
{
  //  TAO_FUNCTION_PP_TIMEPROBE (TAO_GIOP_SEND_MESSAGE_START);

  // Ptr to first buffer.
  char *buf = (char *) stream.buffer ();

  // Length of all buffers.
  size_t total_len =
    stream.total_length ();

  // NOTE: Here would also be a fine place to calculate a digital
  // signature for the message and place it into a preallocated slot
  // in the "ServiceContext".  Similarly, this is a good spot to
  // encrypt messages (or just the message bodies) if that's needed in
  // this particular environment and that isn't handled by the
  // networking infrastructure (e.g., IPSEC).
  
  // Get the header length
  size_t header_len = this->get_header_len ();

  // Get the message size offset
  size_t offset = this->get_message_size_offset ();

  CORBA::ULong bodylen = total_len - header_len;

#if !defined (ACE_ENABLE_SWAP_ON_WRITE)
  *ACE_reinterpret_cast (CORBA::ULong *, buf + offset) = bodylen;
#else
  if (!stream->do_byte_swap ())
    *ACE_reinterpret_cast (CORBA::ULong *,
                           buf + offset) = bodylen;
  else
    ACE_CDR::swap_4 (ACE_reinterpret_cast (char *,
                                           &bodylen),
                     buf + offset);
#endif /* ACE_ENABLE_SWAP_ON_WRITE */

  // Strictly speaking, should not need to loop here because the
  // socket never gets set to a nonblocking mode ... some Linux
  // versions seem to need it though.  Leaving it costs little.
  this->dump_msg ("send",
                  ACE_reinterpret_cast (u_char *,
                                        buf),
                  stream.length ());

  // This guarantees to send all data (bytes) or return an error.
  ssize_t n = transport->send (stub,
                               stream.begin (),
                               max_wait_time);
  
  if (n == -1)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) closing conn %d after fault %p\n",
                    transport->handle (),
                    "GIOP_Message_Factory::send_message ()"));

      return -1;
    }
  
  // EOF.
  if (n == 0)
    {
      if (TAO_orbdebug)
        ACE_DEBUG ((LM_DEBUG,
                    "TAO: (%P|%t) GIOP::send_message () "
                    "EOF, closing conn %d\n",
                    transport->handle()));
      return -1;
    }
  
  return 1;
}

void
TAO_GIOP_Message_Factory::dump_msg (const char */*label*/,
                                    const u_char */*ptr*/,
                                    size_t /*len*/)
{
  if (TAO_debug_level >= 5)
    {
      // I will have to print out all the relevant debug messages!!
      // Let me not wory about that now. I will get back to that at a
      // later date!!
    }
}

  
