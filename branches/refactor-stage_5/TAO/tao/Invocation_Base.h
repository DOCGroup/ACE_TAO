// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Base.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_INVOCATION_BASE_H
#define TAO_INVOCATION_BASE_H
#include "ace/pre.h"

#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Invocation_Utils.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/ClientRequestInfo_i.h"
#include "tao/ClientRequestInterceptor_Adapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

namespace Dynamic
{
  class ExceptionList;
  class ParameterList;
}

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
   * This class is the base of the invocation object hiererachy. This
   * hierarchy is classified  based on the type of invocation and the
   * mode of the invocation. One of the objects from the hiererachy is
   * created on the stack for every invocation.
   *
   * This class encapsulates the essential details that are required
   * for PortableInterceptors to function correctly. Further this
   * class also provides some helper and accessor methods that are
   * used by clients.
   * @@ More..
   */
  class TAO_Export Invocation_Base
  {
  public:
    friend class Invocation_Adapter;

    virtual ~Invocation_Base (void);

    //@{
    /// Accessor and mutator methods ..
    TAO_ORB_Core *orb_core (void) const;
    TAO_Stub *stub (void) const;

    /// Accessor and mutator methods for forwarded object
    /// locations.
    /**
     * These methods have to be public so that the PortableInterceptor
     * can use this.
     */
    CORBA::Object_ptr forwarded_reference (void);
    void forwarded_reference (CORBA::Object_ptr o);

    /// Accessors for the service context list
    TAO_Service_Context &request_service_context (void);
    TAO_Service_Context &reply_service_context (void);

    /// Return the forwarded object location by loosing ownership.
    CORBA::Object_ptr steal_forwarded_reference (void);

    /// Did the invocation got forwarded to a new location?
    bool is_forwarded (void) const;

    /// Mutator to set the reply status of the invocation.
    void reply_received (Invocation_Status s);

    /// Return the effective target of the invocation.
    /**
     * Please see the PortableInterceptor specification in the CORBA
     * spec to understand what effective target means.
     */
    CORBA::Object_ptr effective_target (void) const;

    /// Return the target object
    CORBA::Object_ptr target (void) const;

    /// Does this invocation return a response?
    CORBA::Boolean  response_expected (void) const;

    //@}
  protected:
    /// Don't allow creation of an object of type Invocation_Base.
    /**
     * @param otarget, The original target on which this invocation
     * was started.
     *
     * @param target, the target on which this invocation is flowing
     * ie. the effective target
     *
     * @param op, operation details of the invocation on @a target
     *
     * @param response_Expected, flag to indicate whether the
     * operation encapsulated by @a op returns a response or not.
     */
    Invocation_Base (CORBA::Object_ptr otarget,
                     CORBA::Object_ptr target,
                     TAO_Operation_Details &op,
                     bool response_expected);

  protected:
    /// The operation details on which we are operating on.
    TAO_Operation_Details &details_;

    /// Forwarded object reference.
    CORBA::Object_var forwarded_to_;

    /// Is the response expected?
    bool response_expected_;

  private:

    ACE_UNIMPLEMENTED_FUNC (Invocation_Base & operator= (const Invocation_Base &));

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

    /// Operations invoked by the
    /// PortableInterceptor::ClientRequestInfo object to get details
    /// about the operation and related stuff.
    //@{
#if TAO_HAS_INTERCEPTORS == 1
  public:
    /// Return the name of the operation.
    char *operation_name (void);

    /// Return the list of arguments as a ParameterList
    Dynamic::ParameterList *arguments (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Return the list of exceptions declared  as a ExceptionList
    Dynamic::ExceptionList *exceptions (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Return the result of the operation as an Any.
    CORBA::Any * result (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Return the syncscope policy of the operation.
    CORBA::Octet sync_scope (void) const;

  protected:
    /// Helper method to invoke send_request interception call to all
    /// the registered interceptors.
    Invocation_Status send_request_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Helper method to invoke receive_reply interception call to all
    /// the registered interceptors.
    Invocation_Status receive_reply_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Helper method to invoke receive_other interception call to all
    /// the registered interceptors.
    Invocation_Status receive_other_interception (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

    /// Helper methods to handle interception calls when exceptions
    /// are thrown by the PortableInterceptor.
    PortableInterceptor::ReplyStatus
        handle_any_exception (CORBA::Exception *e
                              ACE_ENV_ARG_DECL);

    PortableInterceptor::ReplyStatus
        handle_all_exception (ACE_ENV_SINGLE_ARG_DECL);

  protected:
    /// The client requestor adapter and the request info object for
    /// making calls on all the registered interceptors.
    ClientRequestInterceptor_Adapter adapter_;
    TAO_ClientRequestInfo_i req_info_;


#endif /*TAO_HAS_INTERCEPTORS*/
  };
}

#if defined (__ACE_INLINE__)
# include "Invocation_Base.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
