// -*- C++ -*-
//$Id$
// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message_Acceptors.h
//
// = DESCRIPTION
//   Interface for the different accept states     
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef TAO_GIOP_MESSAGE_ACCEPT_STATE_H
#define TAO_GIOP_MESSAGE_ACCEPT_STATE_H
#include "tao/GIOP_Server_Request.h"
#include "tao/GIOP_Assorted_Headers.h"

// @@ Bala: do we want to have separate classes for the server side
//    and client side?  IMHO not, with bi-directional connections the
//    differences will be completely blurred.
//    Please think about designs that do not require separate state
//    objects, a good side effect: that should reduce code size...
class TAO_GIOP_Message_Accept_State
{
  // = TITLE
  //   TAO_GIOP_Message_Accept_State
  //
  // = DESCRIPTION
  //   An abstract base class for different versions of GIOP. This is
  //   similar to the base class in the strategy pattern
  //   @@ Bala: Is it an strategy or not!?  
  //  
public:

  virtual int parse_request_header (TAO_GIOP_ServerRequest &) = 0;
  // Parse the Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
  
  virtual CORBA::Boolean write_reply_header (TAO_OutputCDR &output,
                                             CORBA::ULong request_id) = 0;
  // Write the reply header in to <output>

  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &) = 0; 
  // Parse the Loacte Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
 
  virtual CORBA::Boolean 
  write_locate_reply_mesg (TAO_OutputCDR &output,
                           CORBA::ULong request_id,
                           TAO_GIOP_Locate_Status_Msg &status) = 0;
  // Writes the locate _reply message in to the <output>
  
  virtual CORBA::Octet major_version (void) = 0;
  virtual CORBA::Octet minor_version (void) = 0;
  // Our versions

 private:
};

// @@ Bala: again you have the inheritance reversed!  A 1.1 server (or
//    client) must support 1.0, but not vice-versa.
// @@ Bala: a physical design issue: if the protocol is truly
//    pluggable then you should be able to (and you should) put the
//    classes for each protocol in separate files.
class TAO_GIOP_Message_Accept_State_11 :
  public TAO_GIOP_Message_Accept_State
{
  // = TITLE
  //   TAO_GIOP_Message_Accept_State_11
  // = DESCRIPTION
  //   
  
public:
  virtual int parse_request_header (TAO_GIOP_ServerRequest &);
  // Parse the Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header


  virtual CORBA::Boolean  write_reply_header (TAO_OutputCDR &output,
                                              CORBA::ULong request_id);
  // Write the version specific reply header in to <output>
  
  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &);
  // Parse the Loacte Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
  
  virtual CORBA::Boolean 
  write_locate_reply_mesg (TAO_OutputCDR &output,
                           CORBA::ULong request_id,
                           TAO_GIOP_Locate_Status_Msg &status);
  // Writes the locate reply message in to <output>
  
  virtual CORBA::Octet major_version (void);
  virtual CORBA::Octet minor_version (void);
  // Our versions

private:

};

class TAO_GIOP_Message_Accept_State_10 :
  public TAO_GIOP_Message_Accept_State_11
{
  // = TITLE
  // = DESCRIPTION
public:
  virtual CORBA::Octet minor_version (void);
};

class TAO_GIOP_Message_Accept_Impl
{
  // = TITLE
  // = DESCRIPTION
 public:
  CORBA::Boolean check_revision (CORBA::Octet incoming_major,
                                 CORBA::Octet incoming_minor);
  // Performs a check of the revision tags

  TAO_GIOP_Message_Accept_State_10 version_10;
  TAO_GIOP_Message_Accept_State_11 version_11;
  
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Accept_State.i"
#endif /* __ACE_INLINE__ */

#endif /*_TAO_GIOP_MESSAGE_ACCEPT_STATE_H_*/
