// -*- C++ -*-

//=============================================================================
/**
 *  @file   NotifyLogNotification.h
 *
 *  $Id$
 *
 *  Methods to enable log-generated events.
 *
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_NOTIFY_LOG_NOTIFICATION_H
#define TAO_TLS_NOTIFY_LOG_NOTIFICATION_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsNotifyLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/Log/LogNotification.h"
#include "orbsvcs/Log/notifylog_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_NotifyLogNotification
 *
 * @brief Used to forward log generated events to a logging server.
 */
class TAO_NotifyLog_Serv_Export TAO_NotifyLogNotification :
  public TAO_LogNotification,
  public POA_CosNotifyComm::PushSupplier
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_NotifyLogNotification (CosNotifyChannelAdmin::EventChannel_ptr);

  /// Destructor.
  ~TAO_NotifyLogNotification (void);

protected:
  // = Helper methods
  // = Data members

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::ProxyPushConsumer_var proxy_consumer_;

  /// This supplier's id.
  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed);

  // = StructuredPushSupplier method
    virtual void disconnect_push_supplier (void);

  /// Send the event on the event channel.
  virtual void send_notification (const CORBA::Any& any);

private:
  /// A reference to the ORB.
  CORBA::ORB_var orb_;

  /// A pointer to the EventChannel that events are to be sent to.
  /// This channel is contained in the NotifyLogFactory.
  CosNotifyChannelAdmin::EventChannel_var event_channel_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_NOTIFY_LOG_NOTIFICATION_H */
