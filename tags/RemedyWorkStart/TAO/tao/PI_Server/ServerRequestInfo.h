// -*- C++ -*-

//=============================================================================
/**
 * @file ServerRequestInfo.h
 *
 * $Id$
 *
 * This is the implementation of the
 * @c PortableInterceptor::ServerRequestInfo interface.
 *
 * @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 * @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_SERVER_REQUEST_INFO_H
#define TAO_SERVER_REQUEST_INFO_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/PI_Server/PI_Server_includeC.h"
#include "tao/PI/PI.h"
#include "tao/LocalObject.h"
#include "tao/OctetSeqC.h"
#include "tao/TAO_Server_Request.h"
#include "tao/PortableInterceptorC.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServerRequest;
class TAO_Service_Context;

namespace TAO
{
  namespace Portable_Server
  {
    class Servant_Upcall;
  }
}

namespace TAO
{
  /**
   * @class ServerRequestInfo
   *
   * @brief Implementation of the
   *        @c PortableInterceptor::ServerRequestInfo IDL interface.
   *
   * Implementation of the @c PortableInterceptor::ServerRequestInfo
   * IDL interface.
   *
   * @note This class is currently not meant to be reference counted
   *       since it is instantiated on the stack.
   */
  class ServerRequestInfo
    : public virtual PortableInterceptor::ServerRequestInfo
    , public virtual ::CORBA::LocalObject
  {
  public:

    /// Constructor.
    ServerRequestInfo (TAO_ServerRequest & server_request,
                       TAO::Argument * const * args,
                       size_t nargs,
                       TAO::Portable_Server::Servant_Upcall *servant_upcall,
                       CORBA::TypeCode_ptr const * exceptions,
                       CORBA::ULong nexceptions);

    /// Return an ID unique to the current request.  This request ID may
    /// or may not be the same as the GIOP request ID.
    virtual CORBA::ULong request_id (void);

    /// Return the operation name for the current request.
    virtual char * operation (void);

    /// Return the list of arguments passed to the current operation.
    virtual Dynamic::ParameterList * arguments (void);

    /// Return the list of exceptions the current operation is capable
    /// of throwing.
    virtual Dynamic::ExceptionList * exceptions (void);

    virtual Dynamic::ContextList * contexts (void);

    virtual Dynamic::RequestContext * operation_context (void);

    /// Return the result of the current request.  If there is no
    /// return value then an @c Any with @c tk_void @c TypeCode is
    /// returned.  This method is not valid for oneway operations.
    virtual CORBA::Any * result (void);

    /// Returns @c true for a two-way operation, and @c false otherwise.
    virtual CORBA::Boolean response_expected (void);

    /// Return the @c sync_scope policy value for the current one-way
    /// operation.  If the operation is not a one-way, a
    /// @c CORBA::BAD_INV_ORDER exception is thrown.
    virtual Messaging::SyncScope sync_scope (void);

    /// Return the reply status for the current request.
    /**
     * Statuses can be @c PortableInterceptor::SUCCESSFUL,
     * @c SYSTEM_EXCEPTION, @c USER_EXCEPTION, @c LOCATION_FORWARD,
     * @c TRANSPORT_RETRY, @c UNKNOWN..
     */
    virtual PortableInterceptor::ReplyStatus reply_status (void);

    /// If the reply status is
    /// @c PortableInterceptor::LOCATION_FORWARD return the
    /// object reference to which the request was forwarded.
    virtual CORBA::Object_ptr forward_reference (void);

    /// Retrieve data from the "request scope" @c PICurrent object.
    virtual CORBA::Any * get_slot (PortableInterceptor::SlotId id);

    /// Return the @c IOP::ServiceContext with the given
    /// @c IOP::ServiceId from the request service context list.
    virtual IOP::ServiceContext * get_request_service_context (
        IOP::ServiceId id);

    /// Return the @c IOP::ServiceContext with the given
    /// @c IOP::ServiceId from the reply service context list.
    virtual IOP::ServiceContext * get_reply_service_context (
        IOP::ServiceId id);

    /// Return an @c Any containing the exception being sent, if any.
    /// Otherwise, throw a @c CORBA::BAD_INV_ORDER exception.
    /**
     * @note There is no trivial way to extract the exception from an
     *       @c Any.
     */
    virtual CORBA::Any * sending_exception (void);

    /// Return the @c ObjectId for the target object.
    virtual PortableInterceptor::ObjectId * object_id (void);

    /// Return the @c AdapterId for the POA handling the current
    /// request.
    virtual CORBA::OctetSeq * adapter_id (void);

    /// Return the server_id of the server. The value is passed to
    /// the ORB via @c -ORBServerId parameter.
    virtual char * server_id (void);

    /// Return the ORBId value that is passed to the @c ORB_init
    /// call.
    virtual char * orb_id (void);

    /// Return the name of the object adapter that services requests
    /// for the invoked object.
    virtual PortableInterceptor::AdapterName * adapter_name (void);

    /// Return the most derived interface of the target object.
    virtual char * target_most_derived_interface (void);

    /// Return the policy of the given type in effect for the current
    /// request.
    virtual CORBA::Policy_ptr get_server_policy (CORBA::PolicyType type);

    /// Insert data into the "request scope" @c PICurrent object.
    virtual void set_slot (PortableInterceptor::SlotId id,
                           const CORBA::Any & data);

    /// Returns true if the target's type corresponds to the given
    /// @c RepositoryId.
    virtual CORBA::Boolean target_is_a (const char * id);

    /// Add the @c IOP::ServiceContext to the reply (outgoing)
    /// @c IOP::ServiceContextList.
    virtual void add_reply_service_context (
        const IOP::ServiceContext & service_context,
        CORBA::Boolean replace
        );

  public:

    /**
     * @name Helper methods specific to TAO.
     */
    //@{

    /// Extract the forward object reference from the
    /// @c PortableInterceptor::ForwardRequest exception, and set the
    /// reply status flag accordingly.
    void forward_reference (PortableInterceptor::ForwardRequest &exc);

    /// Set the forward reference associated with the current
    /// LOCATION_FORWARD reply.
    /**
     * @note This method is only invoked when a
     * @c PortableServer::ForwardRequest exception is thrown by a
     * servant manager.
     */
    void forward_reference (CORBA::Object_ptr obj);

    //@}

    /// Return a reference to the underlying @c TAO_ServerRequest
    /// object.
    TAO_ServerRequest &server_request (void);

  protected:

    /// Helper method to get the request and response service
    /// contexts.
    IOP::ServiceContext * get_service_context_i (
        TAO_Service_Context & service_context_list,
        IOP::ServiceId id);

  protected:

    /// Underlying request object that contains much of the
    /// information encapsulated by this @c ServerRequestInfo
    /// implementation.
    TAO_ServerRequest & server_request_;

    /// Operation argument list.
    TAO::Argument * const * const args_;

    /// Number of element in the operation argument list.
    size_t const nargs_;

    /// Pointer to the @c Servant_Upcall object that contains the
    /// object ID, among other things.
    TAO::Portable_Server::Servant_Upcall * const servant_upcall_;

    /// Array of @c TypeCodes for user exceptions the operation is
    /// capable of raising.
    CORBA::TypeCode_ptr const * const exceptions_;

    /// The number of elements in the @c exceptions_ array.
    CORBA::ULong const nexceptions_;
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

# if defined (__ACE_INLINE__)
#  include "tao/PI_Server/ServerRequestInfo.inl"
# endif  /* __ACE_INLINE__ */

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_REQUEST_INFO_H */
