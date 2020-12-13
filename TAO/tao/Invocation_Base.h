// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Base.h
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_INVOCATION_BASE_H
#define TAO_INVOCATION_BASE_H

#include /**/ "ace/pre.h"

#include "Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Invocation_Utils.h"
#include "GIOPC.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "Exception.h"
#include "PI_ForwardC.h"
#include "ClientRequestInterceptor_Adapter.h"
#include "ServerRequestInterceptor_Adapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Service_Context;
class TAO_Operation_Details;
class TAO_Stub;

namespace TAO
{
  class Invocation_Adapter;

  /**
   * @class Invocation_Base
   *
   * @brief The base class for the invocation object
   *
   * This class is the base of the invocation object hierarchy. This
   * hierarchy is classified  based on the type of invocation and the
   * mode of invocation. One of the objects from the hierarchy is
   * created on the stack for every invocation.
   *
   * In addition this class encapsulates the essential details that
   * are required for PortableInterceptors to function
   * correctly. Further this class also provides some helper and
   * accessor methods that are used by clients.
   */
  class TAO_Export Invocation_Base
  {
  public:
    virtual ~Invocation_Base ();

    /// Accessor and mutator methods
    //@{
    TAO_Stub *stub () const;

    /// Accessor and mutator methods for forwarded object
    /// locations.
    /**
     * These access methods have to be public so that the
     * PortableInterceptor can use them
     */
    CORBA::Object_ptr forwarded_reference ();
    void forwarded_reference (CORBA::Object_ptr o);

    /// Accessors for the service context list.
    /**
     * The service context lists are actually cached
     * elsewhere. Providing this accessor helps the PI to access this
     * list in both remote and collocated mode.
     */
    TAO_Service_Context &request_service_context ();
    TAO_Service_Context &reply_service_context ();

    /// Return the forwarded object location by loosing ownership.
    CORBA::Object_ptr steal_forwarded_reference ();

    /// Return the effective target of the invocation.
    /**
     * Please see the PortableInterceptor specification in the CORBA
     * spec to understand what effective target means.
     */
    CORBA::Object_ptr effective_target () const;

    /// Return the target object
    CORBA::Object_ptr target () const;

    /// Does this invocation return a response?
    CORBA::Boolean response_expected () const;

    /// Accessor of reply_status of the invocation.
    GIOP::ReplyStatusType reply_status () const;

    /// Mutator of reply_status of the invocation.
    void reply_status (GIOP::ReplyStatusType s);

    /// The operaton details of the invocation
    TAO_Operation_Details &operation_details ();
    //@}

  protected:
    /**
     * @param otarget The original target on which this invocation
     * was started.
     *
     * @param target the target on which this invocation is flowing
     * ie. the effective target
     *
     * @param op operation details of the invocation on @a target
     *
     * @param response_expected flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Invocation_Base (CORBA::Object_ptr otarget,
                     CORBA::Object_ptr target,
                     TAO_Stub *stub,
                     TAO_Operation_Details &op,
                     bool response_expected,
                     bool request_is_remote);

  protected:
    /// The operation details on which we are operating on.
    TAO_Operation_Details &details_;

    /// Forwarded object reference.
    CORBA::Object_var forwarded_to_;

    /// Is response expected?
    bool response_expected_;

    /// A GIOP reply status of the invocation.
    GIOP::ReplyStatusType reply_status_;

  private:
    Invocation_Base (const Invocation_Base&);
    Invocation_Base & operator= (const Invocation_Base &);

  private:
    //@{
    /**
     * The following object reference pointers are *not*
     * duplicated. They are cached for portable interceptors, and they
     * just live for the lifetime of the request. Hence there is no
     * point in duplicating the pointers.
     */
    /// The original target on which the invocation was started.
    CORBA::Object_ptr otarget_;

    /// The effective target on which the invocation is on.
    CORBA::Object_ptr target_;
    //@}

    TAO_Stub *stub_;

    /// Operations invoked by the
    /// PortableInterceptor::ClientRequestInfo object to get details
    /// about the operation and related stuff.
    //@{
#if TAO_HAS_INTERCEPTORS == 1
  public:
    /// Return a reference to the number of interceptors pushed on to
    /// the current interceptor flow stack.
    /**
     * @note It is a reference since the Portable Interceptor flow stack
     *       code must be able to modify this value and use that value
     *       at a later time without being forced to use TSS.
     */
    size_t &stack_size ();

    CORBA::Exception *caught_exception ();

    /// Change the exception status.
    void exception (CORBA::Exception *exception);

    /// Invocation status.
    TAO::Invocation_Status invoke_status () const;
    /// Mutator to set the invocation status.
    void invoke_status (Invocation_Status s);

    PortableInterceptor::ReplyStatus pi_reply_status () const;

    /// Accessor used to determine if the current invocation is part
    /// of a remote request, and if not, it will be considered to be
    /// part of a collocated request.
    bool is_remote_request() const;

  protected:
    /// Helper method to invoke send_request interception call to all
    /// the registered interceptors.
    Invocation_Status send_request_interception ();

    /// Helper method to invoke receive_reply interception call to all
    /// the registered interceptors.
    Invocation_Status receive_reply_interception ();

    /// Helper method to invoke receive_other interception call to all
    /// the registered interceptors.
    Invocation_Status receive_other_interception ();

    /// Helper methods to handle interception calls when exceptions
    /// are thrown by the PortableInterceptor.
    PortableInterceptor::ReplyStatus
        handle_any_exception (CORBA::Exception * e);

    PortableInterceptor::ReplyStatus handle_all_exception ();

  protected:
    /// The client requestor adapter
    ClientRequestInterceptor_Adapter *cri_adapter_;
    ServerRequestInterceptor_Adapter *sri_adapter_;

    size_t stack_size_;

    TAO::Invocation_Status invoke_status_;

  private:
    /// Pointer to the caught exception.
    CORBA::Exception *caught_exception_;

    /// Flag used to distinguish a remote invocation versus a collocated
    /// (thru-poa) invocation.
    bool const is_remote_request_;
#endif /*TAO_HAS_INTERCEPTORS*/
    //@}
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "Invocation_Base.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_INVOCATION_BASE_H*/
