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

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "Service_Context.h"
#include "target_specification.h"

struct TAO_Exception_Data;

namespace CORBA
{
  class Exception;
}

namespace Dynamic
{
  class ParameterList;
  class ExceptionList;
}
namespace TAO
{
  class Argument;
}

/**
 * @class TAO_Operation_Details
 *
 * @brief Class with operation details
 *
 * This class stores some of the "operation" details that would be
 * required by the invocation classes.  This class is in its
 * infancy now but I expect this one to grow as we come with
 * different varieties of use cases.
 */

class TAO_Export TAO_Operation_Details
{
public:

  ///Constructor
  TAO_Operation_Details (const char *name,
                         CORBA::ULong len,
                         CORBA::Boolean argument_flag,
                         TAO::Argument **args = 0,
                         CORBA::ULong num_args = 0,
                         TAO_Exception_Data *ex_data = 0,
                         CORBA::Long ex_count = 0);

  /// Operation name
  const char* opname (void) const;

  /// Length of the operation name
  CORBA::ULong opname_len (void) const;

  /// Return the flag that indicates whether the operation has any
  /// arguments
  CORBA::Boolean argument_flag (void) const;

  /// Set the response flags
  void response_flags (CORBA::Octet flags);

  /// Get the response flags
  CORBA::Octet response_flags (void);
  CORBA::Octet response_flags (void) const;

  /// Get the service context list
  IOP::ServiceContextList &request_service_info (void);
  const IOP::ServiceContextList &request_service_info (void) const;
  IOP::ServiceContextList &reply_service_info (void);
  const IOP::ServiceContextList &reply_service_info (void) const;

  /// Access the TAO_Service_Context
  TAO_Service_Context &request_service_context (void);
  const TAO_Service_Context &request_service_context (void) const;
  TAO_Service_Context &reply_service_context (void);
  const TAO_Service_Context &reply_service_context (void) const;

  void request_id (CORBA::ULong id);

  /// Modify request id's for a BiDirectional setup
  void modify_request_id (int originator);

  /// Return the request ID associated with the operation
  CORBA::ULong request_id (void);
  CORBA::ULong request_id (void) const;

  /// Accessor method for the addressing mode
  TAO_Target_Specification::TAO_Target_Address addressing_mode (void);
  TAO_Target_Specification::TAO_Target_Address
  addressing_mode (void)  const;

  /// Set method for the addressing mode
  void addressing_mode (CORBA::Short addr);

  /// Creates and returns a CORBA::Exception object whose repository
  /// id \a ex matches the exception list that this operation
  /// specified.
  /**
   * This step is important to decode the exception that the client
   * got from the server. If the exception received from the server
   * is not found in the list of exceptions specified by the operation
   * this call would raise an UNKNOWN exception.
   */
  CORBA::Exception *corba_exception (const char *ex
                                     ACE_ENV_ARG_DECL);

  bool marshal_args (TAO_OutputCDR &cdr);
  bool demarshal_args (TAO_InputCDR &cdr);
  bool parameter_list (Dynamic::ParameterList &);
  bool exception_list (Dynamic::ExceptionList &);
  TAO::Argument **args (void);
  CORBA::ULong args_num (void) const ;

private:

  /// Name of the operation being invoked.
  const char *opname_;

  /// Precalculated length of opname_.
  CORBA::ULong opname_len_;

  /// Request ID of this operation.
  CORBA::ULong request_id_;

  /**
   * Flag that indicates whether the operation has any arguments.  If
   * there are any arguments the flag will have a value of 1, 0
   * otherwise.
   */
  CORBA::Boolean argument_flag_;

  /// Response flags
  CORBA::Octet response_flags_;

  /// The ServiceContextList sent to the server side. Only valid
  /// when sending a request.
  TAO_Service_Context request_service_info_;

  /// The ServiceContextList received from the server side. Only
  /// valid when sending a request.
  TAO_Service_Context reply_service_info_;

  /// Addressing  mode for this request.
  TAO_Target_Specification::TAO_Target_Address addressing_mode_;

  TAO::Argument **args_;

  CORBA::ULong num_args_;

  /// The type of exceptions that the operations can throw.
  TAO_Exception_Data *ex_data_;

  /// Count of the exceptions that operations can throw.
  CORBA::ULong ex_count_;
};

#if defined (__ACE_INLINE__)
# include "operation_details.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_OPERATION_DETAILS_H */
