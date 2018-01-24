// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_Request.h
 *
 *  Header file for CORBA's Dynamic Server Skeleton Interface's
 *  "Server Request" type.
 *
 *  @author  Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Chris Cleeland.  Modifications by Aniruddha Gokhale based on CORBAv2.2 Feb 98
 */
//=============================================================================

#ifndef TAO_CORBA_SERVER_REQUEST_H
#define TAO_CORBA_SERVER_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/Context.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// To force execution of the static constructor
// that registers the dynamic service object.
#include "tao/DynamicInterface/Dynamic_Adapter_Impl.h"

#include "tao/TAO_Server_Request.h"
#include "tao/CDR.h"
#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class ServerRequest;
  typedef ServerRequest *ServerRequest_ptr;

  typedef TAO_Pseudo_Var_T<ServerRequest> ServerRequest_var;
  typedef TAO_Pseudo_Out_T<ServerRequest> ServerRequest_out;

  /**
   * @class ServerRequest
   *
   * @brief Class representing the CORBA ServerRequest pseudo-object.
   *
   * Instantiated by the POA for DSI requests and passed up
   * to the application. Contains a reference to the instance
   * of TAO_ServerRequest that is passed up to the POA from
   * the ORB.
   */
  class TAO_DynamicInterface_Export ServerRequest
  {
  public:
    /// Constructor.
    ServerRequest (TAO_ServerRequest &orb_server_request);

    /// Destructor.
    ~ServerRequest (void);

    /// Implementation uses this to provide the ORB with the operation's
    /// parameter list ... on return, their values are available; the
    /// list fed in has typecodes and (perhap) memory assigned.
    void arguments (CORBA::NVList_ptr &list);

    /// Implementation uses this to provide the operation result
    /// ... illegal if exception() was called or params() was not called.
    ///
    /// XXX Implementation should be able to assume response has been
    /// sent when this returns, and reclaim memory it allocated.
    void set_result (const CORBA::Any &value);

    /// Implementation uses this to provide the exception value which is
    /// the only result of this particular invocation.
    ///
    /// XXX Implementation should be able to assume response has been
    /// sent when this returns, and reclaim memory it allocated.
    void set_exception (const CORBA::Any &value);

    // = Get various universal call attributes.

    // e.g., who made the call, the target of the call, what ORB and OA
    // that target object uses.
    //
    // NOTE: none of these report exceptions; unavailability of any of
    // this stuff is a catastrophic error since this is all part of the
    // basic CORBA Object Model.

    /// Marshal outgoing parameters.
    void dsi_marshal (void);

    /// Accessor for the Context member.
    CORBA::Context_ptr ctx (void) const;

    /// Mutator for the Context member.
    void ctx (CORBA::Context_ptr);

    /// Get the operation name.
    const char *operation (void) const;

    // Pseudo object methods.
    static ServerRequest_ptr _duplicate (ServerRequest_ptr);
    static ServerRequest_ptr _nil (void);

    // = Reference counting.
    CORBA::ULong _incr_refcount (void);
    CORBA::ULong _decr_refcount (void);

    /// Set the lazy evaluation flag.
    void _tao_lazy_evaluation (bool lazy_evaluation);

    /// Get the byte order of the incoming CDR stream.
    int _tao_incoming_byte_order (void) const;

    /// Set the byte order of the outgoing CDR stream.
    void _tao_reply_byte_order (int byte_order);

    /// Return a reference to the underlying TAO_ServerRequest object.
    TAO_ServerRequest & _tao_server_request (void);

    /// Returns a user exception through a TAO gateway without
    /// knowing its type.
    void gateway_exception_reply (ACE_CString &raw_exception);

    /// Useful for template programming.
    typedef CORBA::ServerRequest_ptr _ptr_type;
    typedef CORBA::ServerRequest_var _var_type;
    typedef CORBA::ServerRequest_out _out_type;

  private:
    /// If zero then the NVList is evaluated ASAP.
    bool lazy_evaluation_;

    /// Context associated with this request.
    CORBA::Context_ptr ctx_;

    /// Incoming parameters.
    CORBA::NVList_ptr params_;

    /// Return value.
    CORBA::Any_ptr retval_;

    /// Any exception which might be raised.
    CORBA::Any_ptr exception_;

    /// Reference counting.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

    /// Request from the ORB.
    TAO_ServerRequest &orb_server_request_;

    /// Have we sent a user exception obtained from a gateway?
    bool sent_gateway_exception_;
  };
} // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/DynamicInterface/Server_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CORBA_SERVER_REQUEST_H */
