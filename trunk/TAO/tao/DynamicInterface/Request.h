// -*- C++ -*-

//=============================================================================
/**
 *  @file    Request.h
 *
 *  $Id$
 *
 *  Header file for CORBA's Dynamic Invocation Interface "Request"
 *  type.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems, Inc.
 *  @author Additions and RequestSeq by Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_REQUEST_H
#define TAO_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// To force execution of the static constructor
// that registers the dynamic service object.
#include "tao/DynamicInterface/Dynamic_Adapter_Impl.h"

#include "tao/DynamicInterface/ExceptionList.h"

#include "tao/ORB.h"
#include "tao/Environment.h"
#include "tao/CDR.h"
#include "tao/GIOPC.h"
#include "tao/AnyTypeCode/NVList.h"

#if defined (TAO_HAS_AMI)
#include "tao/Messaging/Messaging.h"
#endif /* TAO_HAS_AMI */

#include "ace/SString.h"
#include "ace/Atomic_Op.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_DynamicInterface_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class NVList;
  typedef NVList *NVList_ptr;

  class Context;
  typedef Context *Context_ptr;

  class ContextList;
  typedef ContextList *ContextList_ptr;

  /**
   * @class Request
   *
   * @brief CORBA::Request
   *
   * Provides a way to create requests and populate it with parameters
   * for use in the Dynamic Invocation Interface.
   */
  class TAO_DynamicInterface_Export Request
  {
  public:

    /// Return the target of this request.
    CORBA::Object_ptr target (void) const;

    /// Return the operation name for the request.
    const CORBA::Char *operation (void) const;

    /// Return the arguments for the request.
    CORBA::NVList_ptr arguments (void);

    /// Return the result for the request.
    CORBA::NamedValue_ptr result (void);

    /// Return the exceptions resulting from this request.
    CORBA::ExceptionList_ptr exceptions (void);

    /// Accessor for the Context member.
    CORBA::Context_ptr ctx (void) const;

    /// Mutator for the Context member.
    void ctx (CORBA::Context_ptr);

    /// Return a list of the request's result's contexts.  Since TAO
    /// does not implement Contexts, this will always be 0.
    CORBA::ContextList_ptr contexts (void);

    /**
     * @name Argument manipulation helper functions.
     *
     * Arg adders, one for each type of parameter, with and without
     * optional name. Returns reference to Any for insertion using
     * <<=.
     */
    //@{
    CORBA::Any &add_in_arg (void);
    CORBA::Any &add_in_arg (const char* name);
    CORBA::Any &add_inout_arg (void);
    CORBA::Any &add_inout_arg (const char* name);
    CORBA::Any &add_out_arg (void);
    CORBA::Any &add_out_arg (const char* name);
    //@}

    /// Initialize the return type.
    void set_return_type (CORBA::TypeCode_ptr tc);

    /// Returns reference to Any for extraction using >>=.
    CORBA::Any &return_value (void);

    /// Perform method resolution and invoke an appropriate method.
    /**
     * If the method returns successfully, its result is placed in
     * the result argument specified on @c create_request. The behavior
     * is undefined if this @c Request has already been used with a
     * previous call to @c invoke, @c send, or
     * @c send_multiple_requests.
     *
     * @note A default argument is set, but please note that this not
     *       recommended as the user may not be able to propagate the
     *       exceptions.
     */
    void invoke (void);

    /// Send a oneway request.
    /**
     * @note A default argument is set, but please note that this not
     *       recommended as the user may not be able to propagate the
     *       exceptions.
     */
    void send_oneway (void);

    /**
     * @name The 'deferred synchronous' methods.
     *
     * The 'deferred synchronous' methods.
     */
    //@{
    void send_deferred (void);
    void get_response (void);
    CORBA::Boolean poll_response (void);
    //@}

    /// Callback method for deferred synchronous requests.
    void handle_response (TAO_InputCDR &incoming, GIOP::ReplyStatusType reply_status);

#if defined (TAO_HAS_AMI)
    /// The 'asychronous' send method. The object is a DSI based callback
    /// handler. This handler must implement Messaging::ReplyHandler
    void sendc (CORBA::Object_ptr handler);

    static void _tao_reply_stub (
                    TAO_InputCDR &_tao_reply_cdr,
                    Messaging::ReplyHandler_ptr _tao_reply_handler,
                    CORBA::ULong reply_status);
#endif /* TAO_HAS_AMI */

    /// Pseudo object methods.
    static CORBA::Request* _duplicate (CORBA::Request*);
    static CORBA::Request* _nil (void);

    // = Reference counting.
    CORBA::ULong _incr_refcount (void);
    CORBA::ULong _decr_refcount (void);

    /// Set the lazy evaluation flag.
    void _tao_lazy_evaluation (bool lazy_evaluation);

    /// Get the byte order member.
    int _tao_byte_order (void) const;

    /// Set the byte order member.
    void _tao_byte_order (int byte_order);

    /// Hold on to a user exception in case we are part of a TAO
    /// gateway.
    void raw_user_exception (TAO_InputCDR &cdr);

    /// Accessor for the input stream containing the exception.
    ACE_CString &raw_user_exception (void);

    /// Proprietary method to check whether a response has been
    /// received.
    CORBA::Boolean response_received (void);

    // Useful for template programming.
    typedef CORBA::Request_ptr _ptr_type;
    typedef CORBA::Request_var _var_type;
    typedef CORBA::Request_out _out_type;

  private:
    friend class ::TAO_Dynamic_Adapter_Impl;

    // The following are not allowed except when called from the
    // friend class.
    Request (CORBA::Object_ptr obj,
             CORBA::ORB_ptr orb,
             const CORBA::Char *op,
             CORBA::NVList_ptr args,
             CORBA::NamedValue_ptr result,
             CORBA::Flags flags,
             CORBA::ExceptionList_ptr exceptions);

    Request (CORBA::Object_ptr obj,
             CORBA::ORB_ptr orb,
             const CORBA::Char *op);

    ~Request (void);

  private:

    /// Target object.
    CORBA::Object_ptr target_;

    /// Pointer to our ORB.
    CORBA::ORB_var orb_;

    /// Operation name.
    const char * opname_;

    /// Parameter list.
    CORBA::NVList_ptr args_;

    /// Result of the operation.
    CORBA::NamedValue_ptr result_;

    /// Invocation flags.
    CORBA::Flags flags_;

    /// @deprecated  Holds exceptions.
    // CORBA::Environment env_;

    /// List of exceptions raised by the operation.
    CORBA::ExceptionList_var exceptions_;

    /// List of the request's result's contexts.
    CORBA::ContextList_ptr contexts_;

    /// Context associated with this request.
    CORBA::Context_ptr ctx_;

    /// Reference counting.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

    /// Protect the response_received_.
    TAO_SYNCH_MUTEX lock_;

    /// If not zero then the NVList is not evaluated by default.
    bool lazy_evaluation_;

    /// Set to true upon completion of invoke() or handle_response().
    CORBA::Boolean response_received_;

    /// Can be reset by a gateway when passing along a request.
    int byte_order_;

    /// Stores user exception as a CDR stream when this request is
    /// used in a TAO gateway.
    ACE_CString raw_user_exception_;
  };
} // End CORBA namespace.

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_REQUEST_H */
