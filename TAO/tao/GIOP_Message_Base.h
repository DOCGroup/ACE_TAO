// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message_Base.h
//
// = DESCRIPTION
//     Interface for the GIOP messaging protocol
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.,
//     Based on GIOP.cpp and GIOP.h
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_GIOP_MESSAGE_BASE_H
#define TAO_GIOP_MESSAGE_BASE_H
#include "ace/pre.h"

#include "tao/GIOP_Message_State.h"


class TAO_Export TAO_GIOP_Message_Base : public TAO_Pluggable_Messaging
{
  // = TITLE
  //   Definitions of GIOP specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details common to all the
  //   GIOP versions. Some of them which are here may be shifted if
  //   things start changing between versions

public:
  TAO_GIOP_Message_Base (void);

  virtual ~TAO_GIOP_Message_Base (void);
  // Dtor

  virtual CORBA::Boolean write_protocol_header (TAO_Pluggable_Message_Type t,
                                                TAO_OutputCDR &msg);
  // Writes the GIOP header in to <msg>

  int handle_input (TAO_Transport *transport,
                    TAO_ORB_Core *orb_core,
                    TAO_Message_State_Factory &mesg_state,
                    ACE_Time_Value *max_time_value = 0);
  // Reads input from the transport

  virtual CORBA::Boolean write_message_header   (const TAO_Operation_Details &opdetails,
                                                 TAO_Pluggable_Header_Type header_type,
                                                 TAO_Target_Specification &spec,
                                                 TAO_OutputCDR &msg);
  // Write the  header defined by <header_type> in to <msg>

  int send_message (TAO_Transport *transport,
                    TAO_OutputCDR &stream,
                    ACE_Time_Value *max_wait_time = 0,
                    TAO_Stub *stub = 0,
                    int two_way = 1);
  // Sends the encapsulated stream in <stream> on to the transport

  virtual int parse_reply (TAO_Message_State_Factory &mesg_state,
                           TAO_Pluggable_Reply_Params &params) = 0;
  // Parse the reply message from the server

  int process_client_message (TAO_Transport *transport,
                              TAO_ORB_Core *orb_core,
                              TAO_InputCDR &input,
                              CORBA::Octet message_type) = 0;
  // Processes the messages from the connectors so that they can be
  // passed on to the appropriate states.

protected:

#if 0
  // It would be really cool to have something like this. But, as they
  // are along the critical path such fancy stuff results in reduced
  // performance. But let it me here till we get to a time where in we
  // are forced to use these methods
  // @@ Bala: we will never need this, right?  Each protocol knows how
  //    to parse its messages, right?
  // @@ Carlos: It knows. But what about OMG at one stage deciding to
  //    change things between versions. For example, today the
  //    message_type is the 8th byte in the GIOP header. If they
  //    decide to move that to 10th byte for GIOP2.8. Just
  //    thinking ahead, that wouldn't cost anything today.
  // @@ If they change it we implement that in the GIOP_2_8 class, of
  // course, and BTW, chances that a protocol will be change in such a
  // non-backwards compatible way are slim...
  // @@ Furthermore, why you have this methods here if they are not
  // used?  Do you get paid by the line of code (just kidding ;-)

  size_t header_len (void);
  // This will give the size of the header for different versions of
  // GIOP.

  size_t message_size_offset (void);
  // This will give the message_size offset as specified by different
  // versions of GIOP

  size_t major_version_offset (void);
  // This will give the major_version offset as specified by different
  // versions of GIOP

  size_t minor_version_offset (void);
  // This will give the minor_version offset as specified by different
  // versions of GIOP

  size_t flags_offset (void);
  // This will give the flags  offset as specified by different
  // versions of GIOP

  size_t message_type_offset (void);
  // This will give the message type offset as specified by different
  // versions of GIOP
  // @@The above  methods may not be required. But we have it for a
  // later date use wherein things can changes in GIOP

#endif /*if 0*/

  int  send_error (TAO_Transport *transport);
  // Send error messages

  void dump_msg (const char *label,
                 const u_char *ptr,
                 size_t len);
  // Print out a debug messages..

private:
  virtual CORBA::Octet major_version (void) = 0;
  virtual CORBA::Octet minor_version (void) = 0;
  // These virtual methods need will give the major and minor versions
  // of the GIOP classes that are active.

  virtual CORBA::Boolean
      write_request_header (const TAO_Operation_Details &opdetails,
                            TAO_Target_Specification &spec,
                            TAO_OutputCDR &msg) = 0;
  // Write the GIOP request header in to <msg>

  virtual CORBA::Boolean
      write_locate_request_header (CORBA::ULong request_id,
                                   TAO_Target_Specification &spec,
                                   TAO_OutputCDR &msg) = 0;
  // Write the GIOP locate request header in to <msg>

  virtual int validate_version (TAO_GIOP_Message_State *state) = 0;
  // This will do a validation of the versions that arrive in the transport.

  int parse_header (TAO_GIOP_Message_State *state);
  // Parses the header of the GIOP messages for validity

  virtual int parse_magic_bytes (TAO_GIOP_Message_State *state);
  // validates the first 4 bytes that contain the magic word "GIOP"

  void send_close_connection (const TAO_GIOP_Version &version,
                              TAO_Transport *transport,
                              void *ctx);
  // Close a connection, first sending GIOP::CloseConnection.
};

// @@ Bala: do you need to put this in the header file?
const size_t TAO_GIOP_MESSAGE_HEADER_LEN = 12;
const size_t TAO_GIOP_MESSAGE_SIZE_OFFSET = 8;
const size_t TAO_GIOP_VERSION_MINOR_OFFSET = 5;
const size_t TAO_GIOP_VERSION_MAJOR_OFFSET = 4;
const size_t TAO_GIOP_MESSAGE_FLAGS_OFFSET = 6;
const size_t TAO_GIOP_MESSAGE_TYPE_OFFSET  = 7;

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Base.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_BASE_H*/
