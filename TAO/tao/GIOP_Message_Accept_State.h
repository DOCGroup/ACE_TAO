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
#include "ace/pre.h"
#define TAO_GIOP_MESSAGE_ACCEPT_STATE_H

#include "tao/GIOP_Assorted_Headers.h"


// @@ Bala: do we want to have separate classes for the server side
//    and client side?  IMHO not, with bi-directional connections the
//    differences will be completely blurred.
// @@ Carlos: Yes I agree in a perfect world. But if we look back we
//    see our pluggabl_protocol framework having WELL driven a well
//    defined wedge, which we have to live with. Unless, we think/redo
//    some work on the pluggable_protocol framework to close these
//    gaps, IMHO we need to keep making this division further. But,
//    there is a hack for Bi-Dir GIOP till we solve the above
//    problem. Hack can be applied to specific versions
//    alone. Moreover, most of the protocols like HTTP-NG, HTTP have
//    this server/client differentiation in their basic
//    framework. Please let me know if I am wrong.

//    Please think about designs that do not require separate state
//    objects, a good side effect: that should reduce code size...
// @@ Carlos:I need sometime to think on this. Will surely do.
class TAO_GIOP_ServerRequest;
class TAO_OutputCDR;
class TAO_GIOP_Locate_Status_Msg;

class TAO_GIOP_Message_Accept_State
{
  // = TITLE
  //   TAO_GIOP_Message_Accept_State
  //
  // = DESCRIPTION
  //   Strategy to determine which version of the GIOP request have we
  //   received. This is to aid the server in replying back to the
  //   client in the same version as that of the received request.
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


/*****************************************************************/ 
// @@ Bala: a physical design issue: if the protocol is truly
//    pluggable then you should be able to (and you should) put the
//    classes for each protocol in separate files.
// @@ Carlos: Only GIOP/GIOPlite is pluggable and not the
//    implementation details I think. Does that answer your question?
 
class TAO_GIOP_Message_Accept_State_10 : public TAO_GIOP_Message_Accept_State
{
  // = TITLE
  //   TAO_GIOP_Message_Accept_State_10
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
};

/*****************************************************************/ 
class TAO_GIOP_Message_Accept_State_11: public TAO_GIOP_Message_Accept_State_10
{
  // = TITLE
  //   TAO_GIOP_Message_Accept_State_11
  // = DESCRIPTION
public:
  virtual CORBA::Octet minor_version (void);
};

/*****************************************************************/ 

class TAO_GIOP_Message_Accept_Impl
{
  // = TITLE
  // = DESCRIPTION
  //   The class that would hold the actual references to the concrete
  //   strategy.
 public:
  CORBA::Boolean check_revision (CORBA::Octet incoming_major,
                                 CORBA::Octet incoming_minor);
  // Performs a check of the revision tags

  TAO_GIOP_Message_Accept_State_10 version_10;
  TAO_GIOP_Message_Accept_State_11 version_11;
  // The concrete implementations that we hold
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Accept_State.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*_TAO_GIOP_MESSAGE_ACCEPT_STATE_H_*/
