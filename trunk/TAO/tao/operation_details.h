// This may look like C, but it's really -*- C++ -*-
//     $Id$
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    operation_details.h
//
// = AUTHOR
//    Bala Natarajan <bala@cs.wustl.edu> 
// ============================================================================

#ifndef TAO_OPERATION_DETAILS_H
#define TAO_OPERATION_DETAILS_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/IOPC.h"
#include "tao/target_specification.h"

// @@ Bala: Why is this not part of the RequestHeader?!
class TAO_Export TAO_Operation_Details
{
  // = TITLE
  //   Class with operation details
  //
  // = DESCRIPTION
  //   This class stores some of the "operation" details that would be
  //   required by the invocation classes. this class is in its
  //   infancy now but I expect this one to grow as we come with
  //   different varieties of use cases. 
public:
  TAO_Operation_Details (const char *name,
                         CORBA::ULong len);
  //Ctor
  
  const char* opname (void) const;
  // Operation name
  
  CORBA::ULong opname_len (void) const;
  // Length of the operation name
  
  void response_flags (CORBA::Octet flags);
  // Set the response flags
  
  CORBA::Octet response_flags (void);
  CORBA::Octet response_flags (void) const;
  // Get the response flags

  IOP::ServiceContextList &service_info (void);
  const IOP::ServiceContextList &service_info (void) const;
  // Get the service context list

  void request_id (CORBA::ULong id);

  CORBA::ULong request_id (void);
  CORBA::ULong request_id (void) const;
  // Get for request id
  
  TAO_Target_Specification::TAO_Target_Address addressing_mode (void);
  TAO_Target_Specification::TAO_Target_Address 
  addressing_mode (void)  const;
  // Get method for the addressing mode

  void 
  addressing_mode (CORBA::Short addr);
  // Set method for the addressing mode
  
private:
  const char *opname_;
  // Name of the operation being invoked.

  CORBA::ULong opname_len_;
  // Precalculated length of opname_.

  CORBA::ULong request_id_;
  // Request ID of this operation.
  
  CORBA::Octet response_flags_;
  // Response flags
  
  IOP::ServiceContextList service_info_;
  // The ServiceContextList sent to the server side.  Only valid
  // when sending a request.
  
  // The first element of header is service context list;
  // transactional context would be acquired here using the
  // transaction service APIs.  Other kinds of context are as yet
  // undefined.
  //

  TAO_Target_Specification::TAO_Target_Address addressing_mode_;
  // Addressing mode for this request. 
};

#if defined (__ACE_INLINE__)
# include "tao/operation_details.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_OPERATION_DETAILS_H*/
