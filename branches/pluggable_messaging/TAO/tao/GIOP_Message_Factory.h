// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message.h
//
// = DESCRIPTION
//     Interface for the GIOP messaging protocol
//
// = AUTHOR
//     Copyright 1994-1995 by Sun Microsystems Inc.,
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_MESSAGE_H_
#define _TAO_GIOP_MESSAGE_H_

#include "tao/Pluggable_Messaging.h"
#include "tao/debug.h"
#include "tao/GIOP_Acceptors.h"

class TAO_Export TAO_GIOP_Client_Message_Factory :
  public TAO_Pluggable_Client_Message_Factory
{
  // = TITLE
  //   Definitions of client side of the GIOP specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details common to all the
  //   GIOP versions. Some of them which are here may be shifted if
  //   things start changing between versions

  // IMPORTANT: This code was based on the GIOP.h & GIOP.cpp
  
 public:
  TAO_GIOP_Client_Message_Factory (void);
  
  virtual ~TAO_GIOP_Client_Message_Factory (void);
  // Dtor

  virtual int handle_input (TAO_Transport *transport,
                            TAO_ORB_Core *orb_core,
                            TAO_Message_State_Factory &mesg_state,
                            ACE_Time_Value *max_time_value = 0);
  
  virtual CORBA::Boolean write_request_header (const IOP::ServiceContextList& svc_ctx,
                                               CORBA::ULong request_id,
                                               CORBA::Octet response_flags,
                                               TAO_Target_Specification &spec,
                                               const char* opname,
                                               TAO_OutputCDR &msg);
  // Write the GIOP request header.

  virtual CORBA::Boolean write_locate_request_header (CORBA::ULong request_id,
                                                      TAO_Target_Specification &spec,
                                                      TAO_OutputCDR &msg) = 0;
  // Write the GIOP locate request header.

  virtual CORBA::Boolean start_message (TAO_Pluggable_Message_Type t,
                                        TAO_OutputCDR &msg) = 0;
  // Shouldnt be needed here but for compilation
  
  int send_message (TAO_Transport *transport,
                    TAO_OutputCDR &stream,
                    ACE_Time_Value *max_wait_time = 0,
                    TAO_Stub *stub = 0);

  int parse_reply (TAO_Transport *transport,
                   TAO_Message_State_Factory &mesg_state,
                   IOP::ServiceContextList& reply_ctx,
                   CORBA::ULong &request_id,
                   CORBA::ULong &reply_status) = 0;
  // Parse the reply message from the server

protected:
  virtual const size_t get_header_len (void) = 0;
  // This will give the size of the header for different variants of
  // GIOP. 
  
  virtual const size_t get_message_size_offset (void) = 0;
  // This will give the message_size offset as specified by different
  // variants of GIOP

  virtual int parse_magic_bytes (TAO_InputCDR &input) = 0;
  
  virtual int parse_header (TAO_GIOP_Message_State *state) = 0;
  
};



class TAO_Export TAO_GIOP_Available_States
{
public:
  
  TAO_GIOP_Acceptor_1_1 giop_1_1_;
  // GIOP 1.1 definition

  int check_major (CORBA::Octet &major);
  // Check whether the major revision is loaded
  
  int check_minor (CORBA::Octet &minor);
  // Check whether the minor revision has been loaded

  
};



class TAO_Export TAO_GIOP_Message_Acceptor:
  public TAO_Pluggable_Server_Message_Factory 
{
  // = TITLE
  //   Definitions of server side of the GIOP specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details common to all the
  //   GIOP versions. Some of them which are here may be shifted if
  //   things start changing between versions

  // IMPORTANT: This code was based on the GIOP.h & GIOP.cpp
  
 public:
  TAO_GIOP_Message_Acceptor (void);
  
  virtual ~TAO_GIOP_Message_Acceptor (void);
  // Dtor

  virtual int  handle_input (TAO_Transport *transport,
                             TAO_ORB_Core *orb_core,
                             TAO_Message_State_Factory &state,
                             ACE_Time_Value *max_wait_time = 0);

  // Handles input by reading header, parsing the header, determing
  // what version of GIOP messages we are getting and slo sets the
  // state. Once the state is set, further messages would be processes
  // by the states .
  
  int process_connector_messages (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  CORBA::Octet message_type);
  // Processes the messages from the connectors so that they can be
  // passed on to the appropriate states.

private:
  int read_header (TAO_Transport *transport,
                   TAO_Message_State_Factory *state,
                   CORBA::ULong &header_size,
                   TAO_InputCDR &input,
                   ACE_Time_Value *max_wait_time);
  
  
  int parse_header (TAO_InputCDR &input,
                    TAO_Message_State_Factory *state);

  int parse_magic_bytes (TAO_InputCDR &input,
                         TAO_GIOP_Message_State *state);
  
  ssize_t read_buffer (TAO_Transport *transport,
                       char *buf,
                       size_t len,
                       ACE_Time_Value *max_wait_time);
  // Loop on data read ... this is required since <recv> won't block
  // until the requested amount of data is available.

  int set_state (TAO_GIOP_Message_State *state);
  // Set the state pointer

  TAO_GIOP_Accept_State *accept_states_;
  // Pointer to the states that we will point to.

  TAO_GIOP_Available_States available_states_;
  // The states are available with us. One of the members of this
  // struct would be held in the accept_state_
};


#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Factory.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_MESSAGE_H_*/
