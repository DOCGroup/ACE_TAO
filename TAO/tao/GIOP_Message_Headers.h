// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//   @@ Bala: if you change the name of the file don't forget to
//   change the documentation too (i hate this, IMHO the RCS id is
//   enough, but it is part of the guidelines).
//     GIOP_Assorted_Header.h
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
class TAO_GIOP_Locate_Request_Header
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

  void addressing_disposition (CORBA::Short discriminant);
  // Set the addressing the disposition

  CORBA::Short addressing_disposition (void);
  // Return the addressing disposition

  const GIOP::TargetAddress &target_address (void) const;
  // Get in read mode

  GIOP::TargetAddress &target_address (void);
  // Get in read/write mode

  //  TAO_ObjectKey &object_key_ref (void);
  // Get the Object Key in read/write mode

  TAO_InputCDR &incoming_stream (void);
  // Get the CDR stream for read/write

private:
  CORBA::ULong request_id_;
  // Request id

  CORBA::Short addressing_disposition_;
  // This is the discrminant type for the union encapsulated in
  // TargetAddress.

  GIOP::TargetAddress  target_address_;
  // Introduced by GIOP 1.2. Will not cause us any harm if it is there
  // for other versions.

  TAO_InputCDR *incoming_;
  // Incoming CDR stream
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Headers.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_HEADERS_H*/
