// This may look like C, but it's really -*- C++ -*-
//=============================================================================
/**
 *  @file    operation_details.h
 *
 *  $Id$
 *
 *  @author Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OPERATION_DETAILS_H
#define TAO_OPERATION_DETAILS_H
#include "ace/pre.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "tao/Service_Context.h"
#include "tao/target_specification.h"


// @@ Bala: Why is this not part of the RequestHeader?!
/**
 * @class TAO_Operation_Details
 *
 * @brief Class with operation details
 *
 * This class stores some of the "operation" details that would be
 * required     by the invocation classes. this class is in its
 * infancy now but I expect     this one to grow as we come with
 * different varieties of use cases.
 */

class TAO_Export TAO_Operation_Details
{
public:
  ///Ctor
  TAO_Operation_Details (const char *name,
                         CORBA::ULong len,
                         CORBA::Boolean argument_flag);

  /// Operation name
  const char* opname (void) const;

  /// Length of the operation name
  CORBA::ULong opname_len (void) const;

  /// Return the        flag that indicates whether the operation has any
  /// arguments
  CORBA::Boolean argument_flag (void) const;

  /// Set the response flags
  void response_flags (CORBA::Octet flags);

  /// Get the response flags
  CORBA::Octet response_flags (void);
  CORBA::Octet response_flags (void) const;

  /// Get the service context list
  IOP::ServiceContextList &service_info (void);
  const IOP::ServiceContextList &service_info (void) const;

  /// Access the TAO_Service_Context
  TAO_Service_Context &service_context (void);
  const TAO_Service_Context &service_context (void) const;

  void request_id (CORBA::ULong id);

  /// Modify request id's for a BiDirectional setup
  void modify_request_id (int flag);

  /// Get for request id
  CORBA::ULong request_id (void);
  CORBA::ULong request_id (void) const;



  /// Get method        for the addressing mode
  TAO_Target_Specification::TAO_Target_Address addressing_mode (void);
  TAO_Target_Specification::TAO_Target_Address
  addressing_mode (void)  const;

  /// Set method for the addressing mode
  void
  addressing_mode (CORBA::Short addr);

private:
  /// Name of the operation being invoked.
  const char *opname_;

  /// Precalculated length of opname_.
  CORBA::ULong opname_len_;

  /// Request ID of this operation.
  CORBA::ULong request_id_;

  /**
   * Flag that indicates whether the operation has any arguments. If
   * there are any arguments the flag will have a value of 1, 0
   * otherwise.
   */
  CORBA::Boolean argument_flag_;

  /// Response flags
  CORBA::Octet response_flags_;

  /// The ServiceContextList sent to the server side.  Only valid
  /// when sending a request.
  TAO_Service_Context service_info_;

  // The first element of header is service context list;
  // transactional context would be acquired here using the
  // transaction service APIs.  Other kinds of context are as yet
  // undefined.
  //

  /// Addressing  mode for this request.
  TAO_Target_Specification::TAO_Target_Address addressing_mode_;
};

#if defined (__ACE_INLINE__)
# include "tao/operation_details.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_OPERATION_DETAILS_H*/
