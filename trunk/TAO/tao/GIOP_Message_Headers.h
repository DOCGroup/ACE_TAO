// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Message_Headers.h
//
// = DESCRIPTION
//     Some assorted GIOP structure mappings
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_GIOP_MESSAGE_HEADERS_H
#define TAO_GIOP_MESSAGE_HEADERS_H
#include "ace/pre.h"

#include "tao/GIOPC.h"

// @@ Bala: what goes in this file? All the message headers? A few of
// @@ them? Only the Locate_Request header?!

// @@ Bala: the TAO_Export macros are missing here too!
class TAO_Export TAO_GIOP_Locate_Request_Header
{
  // = TITLE
  //   Location service support
public:

  TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg);
  // Constructor

  void request_id (CORBA::ULong id);
  // Set the id

  CORBA::ULong request_id (void);
  // Get the request id

  const TAO_ObjectKey &object_key (void) const;
  // Get the object_key in read mode..

  TAO_ObjectKey &object_key (void);
  // Get the object_key in read/write mode..

  IOP::TaggedProfile &tagged_profile (void);
  // get the tagged_profile in the read/write mode

  GIOP::IORAddressingInfo &addressing_info (void);
  // get the addressing info in the read/write mode

  TAO_InputCDR &incoming_stream (void);
  // Get the CDR stream for read/write

private:
  CORBA::ULong request_id_;
  // Request id

  TAO_ObjectKey object_key_;
  // Object Key

  IOP::TaggedProfile profile_;
  // The Tagged profile. This class would have the Tagged Profile

  GIOP::IORAddressingInfo addr_info_;
  // The addressing info

  // The above two declarations are not used in TAO as on date. It is
  // here so that we can use this if we get to that point. The
  // object_key extracted  from these would still be available in
  // <object_key_>. 
  
  TAO_InputCDR *incoming_;
  // Incoming CDR stream
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Headers.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_HEADERS_H*/
