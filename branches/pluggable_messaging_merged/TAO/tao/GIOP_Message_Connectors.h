// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message_Connectors.h
//
// = DESCRIPTION
//     
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_MESSAGE_CONNECTORS_
#define _TAO_GIOP_MESSAGE_CONNECTORS_
#include "tao/GIOP_Message_Base.h"

class TAO_Export TAO_GIOP_Message_Connectors:
  public TAO_GIOP_Message_Base
{
  // = TITLE
  // = DESCRIPTION
  
  int process_connector_messages (TAO_Transport *transport,
                                  TAO_ORB_Core *orb_core,
                                  TAO_InputCDR &input,
                                  CORBA::Octet message_type);
  // Processes the messages from the connectors so that they can be
  // passed on to the appropriate states.

protected:
  virtual CORBA::Boolean 
  write_request_header (const IOP::ServiceContextList& svc_ctx, 
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification &spec,
                        const char* opname,
                        TAO_OutputCDR &msg);
  // Write the GIOP request header.
  
  int parse_reply (TAO_Message_State_Factory &mesg_state,
                   TAO_Pluggable_Connector_Params &params,
                   CORBA::ULong &reply_status);
  // Parse the reply message from the server

private:
  virtual CORBA::Boolean 
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg) = 0;
  


  int validate_version (TAO_GIOP_Message_State *state);
  
  CORBA::Octet major_version (void) = 0;
  CORBA::Octet minor_version (void) = 0;
};

class TAO_Export TAO_GIOP_Message_Connector_11:
  public TAO_GIOP_Message_Connectors
{
  // = TITLE
  // = DESCRIPTION
public:

private:
  virtual CORBA::Boolean 
  write_request_header (const IOP::ServiceContextList& svc_ctx, 
                        CORBA::ULong request_id,
                        CORBA::Octet response_flags,
                        TAO_Target_Specification &spec,
                        const char* opname,
                        TAO_OutputCDR &msg);

  CORBA::Boolean
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg);
  
  int parse_reply (TAO_Message_State_Factory &mesg_state,
                   TAO_Pluggable_Connector_Params &params,
                   CORBA::ULong &reply_status);

  CORBA::Octet major_version (void);
  CORBA::Octet minor_version (void);
};

class TAO_Export TAO_GIOP_Message_Connector_10:
  public TAO_GIOP_Message_Connector_11
{
  // = TITLE
  // = DESCRIPTION
public:

private:
  CORBA::Octet minor_version (void);
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Connectors.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_MESSAGE_CONNECTORS_*/
