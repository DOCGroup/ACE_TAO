// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     GIOP_Assorted_Header.h
//
// = DESCRIPTION
//     Some assorted GIOP structure mappings
//
// = AUTHOR
//     Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================
#ifndef TAO_GIOP_ASSORTED_HEADERS_H
#define TAO_GIOP_ASSORTED_HEADERS_H
#include "tao/GIOPC.h"

// @@ Bala, please pick the names of your files better.
//    First there is only one header here.  But GIOP_Headers is better
//    (i.e. sounds more profesional) than 'assorted' (that sounds like
//    a recipe for vegetable soup)

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
# include "tao/GIOP_Assorted_Headers.i"
#endif /* __ACE_INLINE__ */

#endif /*TAO_GIOP_ASSORTED_HEADERS_H*/
