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

#ifndef TAO_GIOP_MESSAGE_LOCATE_HEADER_H
#define TAO_GIOP_MESSAGE_LOCATE_HEADER_H
#include "ace/pre.h"

#include "tao/Tagged_Profile.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_Export TAO_GIOP_Locate_Request_Header
{
  // = TITLE
  //   Location service support
public:

  TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg,
                                  TAO_ORB_Core *core);
  // Constructor

  void request_id (CORBA::ULong id);
  // Set the id

  CORBA::ULong request_id (void);
  // Get the request id

  const TAO_ObjectKey &object_key (void) const;
  // Get the object_key in read mode..

  TAO_ObjectKey &object_key (void);
  // Get the object_key in read/write mode..

  TAO_Tagged_Profile &profile (void);
  // Get the reference to the underlying profile

  TAO_InputCDR &incoming_stream (void);
  // Get the CDR stream for read/write

private:
  CORBA::ULong request_id_;
  // Request id

  TAO_Tagged_Profile profile_;
  // Profile info.

  TAO_ObjectKey object_key_;
  // Object Key

  TAO_InputCDR *incoming_;
  // Incoming CDR stream
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Headers.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_LOCATE_HEADER_H*/
