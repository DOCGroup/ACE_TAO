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
//   Interface for the client side of the GIOP classes
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_GIOP_MESSAGE_CONNECTORS_H
#define TAO_GIOP_MESSAGE_CONNECTORS_H
#include "ace/pre.h"

#include "tao/GIOP_Message_Base.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// @@ Bala: why are this class called "Connector" does it have
// @@ anything to do with establishing connection? It looks more like
// @@ request parsing or something like that!

class TAO_Export TAO_GIOP_Message_Connectors: public TAO_GIOP_Message_Base
{
  // = TITLE
  //   TAO_GIOP_Message_Connectors

  // = DESCRIPTION
  //   This class provides methods and code for the connector specific
  //   functionality of GIOP.The motivation for this class is to hold
  //   common code between different versions of GIOP in a single
  //   class. 
public:
  
  CORBA::Boolean write_reply_header (TAO_OutputCDR &cdr,
                                     TAO_Pluggable_Reply_Params &reply,
                                     CORBA::Environment &ACE_TRY_ENV =
                                     TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This would not be used now. So it will be returning an error
  // message. 
  // @@ This will be implemented once we get to the Bi-Dir GIOP.  
  //   class.

protected:
  int parse_reply (TAO_Message_State_Factory &mesg_state,
                   TAO_Pluggable_Reply_Params &params);
  // Parse the reply message from the server

private:
  virtual CORBA::Boolean
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg) = 0;
  // Need not be here. But just for the sake of documentaion.


  int validate_version (TAO_GIOP_Message_State *state);

  CORBA::Octet major_version (void) = 0;
  CORBA::Octet minor_version (void) = 0;
  // Virtual methods that will be implemented by the version specific
  // class. There may be a feeling that this declaration may not be
  // required, but some of the code in the class look for this method

  int process_client_message (TAO_Transport *transport,
                              TAO_ORB_Core *orb_core,
                              TAO_InputCDR &input,
                              CORBA::Octet message_type);
  // Processes the messages from the connectors so that they can be
  // passed on to the appropriate states.
};

//////////////////////////////////////////////////
// Version specific classes of GIOP
//////////////////////////////////////////////////

// ****************************************************************

class TAO_Export TAO_GIOP_Message_Connector_10: public TAO_GIOP_Message_Connectors
{
  // = TITLE
  //   TAO_GIOP_Message_Connector_11
  // = DESCRIPTION
  //   This class provides methods methods that implements the GIOP
  //   (1.0) specific stuff.
public:

  TAO_GIOP_Message_Connector_10 (void);
  // Ctor
  
private:
  virtual CORBA::Boolean 
  write_request_header (const TAO_Operation_Details &opdetails,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg);
  // Write the request header in to <msg>

  CORBA::Boolean
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg);
  // Write the locate request header in to the <msg>

  int parse_reply (TAO_Message_State_Factory &mesg_state,
                   TAO_Pluggable_Reply_Params &params);
  // Parse the reply messages from the server

  CORBA::Octet major_version (void);
  CORBA::Octet minor_version (void);
  // Our minor and major versions
};

// ****************************************************************

class TAO_Export TAO_GIOP_Message_Connector_11: public TAO_GIOP_Message_Connector_10
{
  // = TITLE
  //   TAO_GIOP_Message_Connector_11
  // = DESCRIPTION
  //   This class is deribed from the 1.0 version of GIOP. At presnt
  //   this only returns the right version number
public:

  TAO_GIOP_Message_Connector_11 (void);
  // Ctor

private:
  CORBA::Octet minor_version (void);
  // Our minor version
};

/*************************************************************/
class TAO_Export TAO_GIOP_Message_Connector_12: public TAO_GIOP_Message_Connectors
{
  // = TITLE
  //   TAO_GIOP_Message_Connector_12
  // = DESCRIPTION
  //   This class is deribed from the 1.0 version of GIOP. At presnt
  //   this only returns the right version number

public:

  TAO_GIOP_Message_Connector_12 (void);
  // Ctor
  
private:
  CORBA::Boolean 
  write_request_header (const TAO_Operation_Details &opdetails,
                        TAO_Target_Specification &spec,
                        TAO_OutputCDR &msg);
  // Write the request header in to <msg>

  CORBA::Boolean
  write_locate_request_header (CORBA::ULong request_id,
                               TAO_Target_Specification &spec,
                               TAO_OutputCDR &msg);
  // Write the locate request header in to the <msg>
  
  int parse_reply (TAO_Message_State_Factory &mesg_state,
                   TAO_Pluggable_Reply_Params &params);
  // Parse the reply messages from the server

  CORBA::Octet major_version (void);
  CORBA::Octet minor_version (void);
  // Our minor and major versions  
  
  CORBA::Boolean marshall_target_spec (TAO_Target_Specification &spec,
                                       TAO_OutputCDR &msg);
  // This is a helper method that would be used by the method,
  // write_request_header (). This method extracts the the correct
  // specification details from <spec> and marshalls the details in to
  // <msg> 

};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Connectors.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_CONNECTORS_H*/
