// -*- C++ -*-

//=============================================================================
/**
 *  @file     GIOP_Message_Locate_Header.h
 *
 *  $Id$
 *
 *   Some assorted GIOP structure mappings
 *
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_GIOP_MESSAGE_LOCATE_HEADER_H
#define TAO_GIOP_MESSAGE_LOCATE_HEADER_H
#include "ace/pre.h"

#include "tao/Tagged_Profile.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_GIOP_Locate_Request_Header
 *
 * @brief Location service support
 */
class TAO_Export TAO_GIOP_Locate_Request_Header
{
public:

  /// Constructor
  TAO_GIOP_Locate_Request_Header (TAO_InputCDR &msg,
                                  TAO_ORB_Core *core);

  /// Set the id
  void request_id (CORBA::ULong id);

  /// Get the request id
  CORBA::ULong request_id (void);

  /// Get the object_key in read mode..
  const TAO_ObjectKey &object_key (void) const;

  /// Get the object_key in read/write mode..
  TAO_ObjectKey &object_key (void);

  /// Get the reference to the underlying profile
  TAO_Tagged_Profile &profile (void);

  /// Get the CDR stream for read/write
  TAO_InputCDR &incoming_stream (void);

private:
  /// Request id
  CORBA::ULong request_id_;

  /// Profile info.
  TAO_Tagged_Profile profile_;

  /// Object Key
  TAO_ObjectKey object_key_;

  /// Incoming CDR stream
  TAO_InputCDR *incoming_;
};

#if defined (__ACE_INLINE__)
# include "tao/GIOP_Message_Locate_Header.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_GIOP_MESSAGE_LOCATE_HEADER_H*/
