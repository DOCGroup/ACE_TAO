// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Acceptors.h
//
// = DESCRIPTION
//     Concrete classes for the different versions of GIOP classes 
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef _TAO_GIOP_ACCEPTORS_H_
#define _TAO_GIOP_ACCEPTORS_H_
#include "tao/GIOP_Accept_State.h"
#include "tao/GIOP_Server_Request.h"


class TAO_Export TAO_GIOP_Acceptor_1_1: 
public TAO_GIOP_Accept_State
{
  // = TITLE
  //   Definitions of GIOP 1.1 specific stuff
  //
  // = DESCRIPTION
  //   This class will hold the specific details of 1.1 
  //  
 public:
  
  TAO_GIOP_Acceptor_1_1 (void);
  // Ctor

  virtual int parse_request_header (TAO_GIOP_ServerRequest &request);
  // Parse the Request Header. 

  virtual int parse_locate_header (TAO_GIOP_Locate_Request_Header &request); 
  // Parse the Locate  Request Header. 

  virtual int make_reply (const TAO_GIOP_Version &version,
                          TAO_GIOP_Message_Type t,
                          CORBA::ULong request_id,
                          TAO_OutputCDR &output);
  // Make the GIOP header & reply header and will be version
  // specific. 

  const CORBA::Octet& major_version (void);
  const CORBA::Octet& minor_version (void);
  // Return the reference to the major and minor revisions
  
  virtual size_t header_length (void);
  // Returns the header length

  virtual size_t offset_length (void);
  // Returns the message offset

 private:
  const CORBA::Octet our_major_version_;
  const CORBA::Octet our_minor_version_;
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Acceptors.i"
#endif /* __ACE_INLINE__ */
#endif /*_TAO_GIOP_ACCEPTORS_H_*/
