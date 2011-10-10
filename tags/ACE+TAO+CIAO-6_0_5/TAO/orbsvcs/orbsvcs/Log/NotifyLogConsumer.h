// -*- C++ -*-

//=============================================================================
/**
 *  @file   NotifyLogConsumer.h
 *
 *  $Id$
 *
 *  The NotifyLogConsumer connects to the NotifyLog and logs the events
 *  that are pushed to the NotifyLog.
 *
 *  @author David A. Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_NOTIFYLOGCONSUMER_H
#define TAO_TLS_NOTIFYLOGCONSUMER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/DsLogAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/DsNotifyLogAdminS.h"
#include "orbsvcs/Log/Log_i.h"

#include "orbsvcs/Log/notifylog_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LogMgr_i;
class TAO_NotifyLog_i;
class TAO_NotifyLogFactory_i;

/**
 * @class TAO_Notify_LogConsumer
 *
 * @brief An implementation of the PushConsumer interface.
 *
 * Connects to the NotifyLog and writes LogRecords to the
 * Log when events are 'pushed' to the NotifyLog which is an
 * EventChannel.
 */
class TAO_Notify_LogConsumer :
  public virtual POA_CosNotifyComm::PushConsumer
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Notify_LogConsumer (TAO_NotifyLog_i *log);

  /// Connect to NotifyLog.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin);

  /// Disconnect from NotifyLog.
  virtual void disconnect (void);

protected:
  /// Destructor.
  virtual ~TAO_Notify_LogConsumer (void);

  // = NotifyPublish method
  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
    );

  // = PushSupplier methods
  virtual void push (const CORBA::Any & event);

  virtual void disconnect_push_consumer (void);

  /// ProxyPushSupplier used to connect to NotifyLog.
  CosNotifyChannelAdmin::ProxyPushSupplier_var proxy_supplier_;

  /// The ID of the proxySupplier.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  /// The NotifyLog that this consumer connects to
  /// and whose events are to be logged.
  TAO_NotifyLog_i *log_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_NOTIFYLOGCONSUMER_H */
