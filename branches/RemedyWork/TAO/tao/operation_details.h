// -*- C++ -*-
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

#include /**/ "ace/pre.h"

#include "tao/Service_Context.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Exception.h"
#include "tao/TimeBaseC.h"
#include "tao/target_specification.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations
namespace Dynamic
{
  class ParameterList;
  class ExceptionList;
}

namespace TAO
{
  class Argument;
  class Collocated_Arguments_Converter;
  struct Exception_Data;
}

namespace TAO
{
  namespace CSD
  {
    class FW_Server_Request_Wrapper;
  }
}

class TAO_Reply_Dispatcher;

/**
 * @class TAO_Operation_Details
 *
 * @brief Class with operation details
 *
 * This class stores some of the "operation" details that would be
 * required by the invocation classes.  This class is in its
 * infancy now but I expect this one to grow as we come with
 * different varieties of use cases.
 *
 * @todo Put this in namespace TAO.
 */
class TAO_Export TAO_Operation_Details
{
public:

  /// Declare FW_Server_Request_Wrapper a friend
  /// This friendship makes the FW_Server_Request_Wrapper be able to
  /// clone the TAO_Operation_Details data member in TAO_ServerRequest.
  friend class TAO::CSD::FW_Server_Request_Wrapper;

  /// Constructor
  TAO_Operation_Details (const char *name,
                         CORBA::ULong len,
                         TAO::Argument **args = 0,
                         CORBA::ULong num_args = 0,
                         TAO::Exception_Data *ex_data = 0,
                         CORBA::ULong ex_count = 0);

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
  CORBA::Octet response_flags (void) const;

  /// Accessors for the service context list
  IOP::ServiceContextList &request_service_info (void);
  const IOP::ServiceContextList &request_service_info (void) const;
  IOP::ServiceContextList &reply_service_info (void);
  const IOP::ServiceContextList &reply_service_info (void) const;

  /// Access the TAO_Service_Context
  TAO_Service_Context &request_service_context (void);
  const TAO_Service_Context &request_service_context (void) const;
  TAO_Service_Context &reply_service_context (void);
  const TAO_Service_Context &reply_service_context (void) const;

  /// Reset the contents of the service context lists that we hold.
  void reset_request_service_info (void);
  void reset_reply_service_info (void);

  /// Cache the request id.
  void request_id (CORBA::ULong id);

  /// Return the request ID associated with the operation
  CORBA::ULong request_id (void) const;

  /// Accessor method for the addressing mode
  TAO_Target_Specification::TAO_Target_Address addressing_mode (void) const;

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
  CORBA::Exception *corba_exception (const char *ex) const;

  /// Check whether exception @a ex is in the signature of this operation
  bool has_exception (::CORBA::Exception& ex) const;

  /**
   * @name Helper methods used by the Invocation classes.
   */
  //@{

  /// Marshals the list of @c this->arg_ into the \a cdr.
  bool marshal_args (TAO_OutputCDR &cdr);

  /// Demarshals the list of @c this->arg_ into the \a cdr.
  bool demarshal_args (TAO_InputCDR &cdr);

  /// Accessors for the argument list
  TAO::Argument ** args (void) const;
  CORBA::ULong args_num (void) const ;

  /// Exception count
  CORBA::ULong ex_count (void) const;

  TAO::Exception_Data const * ex_data (void) const;

  CORBA::Boolean use_stub_args (void) const;
  void use_stub_args (CORBA::Boolean use_stub_arguments);

#if TAO_HAS_INTERCEPTORS == 1
  void ft_expiration_time (TimeBase::TimeT time);
  TimeBase::TimeT ft_expiration_time (void) const;

  void ft_retention_id (CORBA::Long request_id);
  CORBA::Long ft_retention_id (void) const;
#endif /*TAO_HAS_INTERCEPTORS == 1*/

  /// Accessor for cac_ pointer.
  TAO::Collocated_Arguments_Converter *cac (void) const;
  void cac (TAO::Collocated_Arguments_Converter *cac);

  TAO_Reply_Dispatcher *reply_dispatcher (void) const;
  void reply_dispatcher (TAO_Reply_Dispatcher *rd);

private:

  /// Name of the operation being invoked.
  const char *opname_;

  /// Precalculated length of opname_.
  CORBA::ULong opname_len_;

  /// Request ID of this operation.
  CORBA::ULong request_id_;

  /// Response flags
  CORBA::Octet response_flags_;

  /// The ServiceContextList sent to the server side. Only valid
  /// when sending a request.
  TAO_Service_Context request_service_info_;

  /// The ServiceContextList received from the server side. Only
  /// valid when sending a request.
  TAO_Service_Context reply_service_info_;

  /// Addressing mode for this request.
  TAO_Target_Specification::TAO_Target_Address addressing_mode_;

  /**
   * The arguments of this operation. Note that the return value is also
   * an argument
   */
  TAO::Argument **args_;

  /// Number of arguments including the return value
  CORBA::ULong num_args_;

  /// The type of exceptions that the operations can throw.
  TAO::Exception_Data *ex_data_;

  /// Count of the exceptions that operations can throw.
  CORBA::ULong ex_count_;

  /// Boolean flag to indicate whether in the skeletons the stub arguments
  /// stored in these operation details should be used or not.
  CORBA::Boolean use_stub_args_;

#if TAO_HAS_INTERCEPTORS == 1
  /// FT request expiration time (absolute gregorian)
  TimeBase::TimeT ft_expiration_time_;

  /// FT request retention id
  CORBA::Long ft_retention_id_;
#endif /*TAO_HAS_INTERCEPTORS == 1*/

  /// The optional collocated arguments converter
  TAO::Collocated_Arguments_Converter *cac_;

  /// The optional reply dispatcher
  TAO_Reply_Dispatcher *reply_dispatcher_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/operation_details.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_OPERATION_DETAILS_H */
