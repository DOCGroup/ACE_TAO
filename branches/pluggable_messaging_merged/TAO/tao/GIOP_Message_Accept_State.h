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
//     
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
// ============================================================================
#ifndef _TAO_GIOP_MESSAGE_ACCEPT_STATE_H_
#define _TAO_GIOP_MESSAGE_ACCEPT_STATE_H_
#include "tao/GIOP_Server_Request.h"
#include "tao/GIOP_Assorted_Headers.h"


class TAO_GIOP_Message_Accept_State
{
  // = TITLE
  // = DESCRIPTION
 public:

  virtual int parse_request_header (TAO_GIOP_ServerRequest &) = 0;
  // Parse the Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
  
  virtual CORBA::Boolean write_reply_header (TAO_OutputCDR &output,
                                             CORBA::ULong request_id) = 0;

  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &) = 0; 
  // Parse the Loacte Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
 
  virtual CORBA::Boolean 
  write_locate_reply_mesg (TAO_OutputCDR &output,
                           CORBA::ULong request_id,
                           TAO_GIOP_Locate_Status_Msg &status) = 0;
  
  virtual CORBA::Octet major_version (void) = 0;
  virtual CORBA::Octet minor_version (void) = 0;
 private:
  
};

class TAO_GIOP_Message_Accept_State_11 :
  public TAO_GIOP_Message_Accept_State
{
  // = TITLE
  // = DESCRIPTION
  
public:
  virtual int parse_request_header (TAO_GIOP_ServerRequest &);
  // Parse the Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header


  virtual CORBA::Boolean  write_reply_header (TAO_OutputCDR &output,
                                              CORBA::ULong request_id);
  
  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &);
  // Parse the Loacte Request Header from the incoming stream. This will do a
  // version specific parsing of the incoming Request header
  
  virtual CORBA::Boolean 
  write_locate_reply_mesg (TAO_OutputCDR &output,
                           CORBA::ULong request_id,
                           TAO_GIOP_Locate_Status_Msg &status);
  
  virtual CORBA::Octet major_version (void);
  virtual CORBA::Octet minor_version (void);
  
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
