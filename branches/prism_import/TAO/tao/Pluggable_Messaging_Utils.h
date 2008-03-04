// -*- C++ -*-

//=============================================================================
/**
 *  @file     Pluggable_Messaging_Utils.h
 *
 *  $Id$
 *
 *   Utility classes for the TAO pluggable messaging framework.
 *
 *  @author  Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PLUGGABLE_MESSAGING_UTILS_H
#define TAO_PLUGGABLE_MESSAGING_UTILS_H

#include /**/ "ace/pre.h"

#include "tao/IOP_IORC.h"
#include "tao/GIOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;

/**
 * @class TAO_Pluggable_Reply_Params_Base
 *
 * @brief TAO_Pluggable_Reply_Params_Base
 *
 * This represents a set of data that would be assembled by the
 * acceptor to pass to the connector. This base class is used by
 * TAO_ServerRequest. The child class TAO_Pluggable_Reply_Params
 * is used on the client side, and contains an additional
 * TAO_InputCDR member, not needed on the server side.
 */
class TAO_Export TAO_Pluggable_Reply_Params_Base
{
public:
  /// Constructor.
  TAO_Pluggable_Reply_Params_Base (void);

  /// The IOP service context list.
  IOP::ServiceContextList svc_ctx_;

  /// The request id for which the reply we (connector) has received.
  CORBA::ULong request_id_;

  /**
   * Since this class no longer contains an NVList, this is the
   * way to determine if the request was DSI, so we can use Carlos'
   * service context list no-deep-copy optimization.
   */
  CORBA::Boolean is_dsi_;

  /// Info required for DSI optimization that pads the outgoing
  /// CDR stream according to the alignment of the NVList.
  ptrdiff_t dsi_nvlist_align_;

  /**
   * Get and Set methods for the service context list that we dont
   * own. This is useful for cases  where the application objects own
   * a service context list and would like to pass on their contents
   * without a copy.
   */
  IOP::ServiceContextList &service_context_notowned (void);
  void service_context_notowned (IOP::ServiceContextList *svc);

  /// A flag that indicates if there is any data is going to get
  /// marshalled in the reply
  CORBA::Boolean argument_flag_;

  GIOP::ReplyStatusType reply_status (void) const;
  void reply_status (GIOP::ReplyStatusType status);

  GIOP::LocateStatusType locate_reply_status (void) const;
  void locate_reply_status (GIOP::LocateStatusType status);

protected:
  /// The service context list that we don't own.
  IOP::ServiceContextList *service_context_;
private:
  /// The reply status.
  GIOP::ReplyStatusType reply_status_;

  /// The locate reply status
  GIOP::LocateStatusType locate_reply_status_;

};

/**
 * @class TAO_Pluggable_Reply_Params
 *
 * @brief TAO_Pluggable_Connector_Params
 *
 */
class TAO_Export TAO_Pluggable_Reply_Params
  : public TAO_Pluggable_Reply_Params_Base
{
public:
  /// Constructor.
  TAO_Pluggable_Reply_Params (TAO_Transport *t);

  /// The stream with the non-demarshaled reply. This stream will be
  /// passed up to the stubs to demarshal the parameter values.
  TAO_InputCDR *input_cdr_;

  TAO_Transport *transport_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Pluggable_Messaging_Utils.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_PLUGGABLE_MESSAGING_UTILS_H */
