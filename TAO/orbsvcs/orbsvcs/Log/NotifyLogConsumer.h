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

#include "notifylog_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

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
  public virtual POA_CosNotifyComm::StructuredPushConsumer,
  public PortableServer::RefCountServantBase
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_Notify_LogConsumer (TAO_NotifyLog_i *log);

  /// Connect to NotifyLog.
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL);

  /// Disconnect from NotifyLog.
  virtual void disconnect (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// Destructor.
  virtual ~TAO_Notify_LogConsumer (void);

  // = NotifyPublish method
  virtual void offer_change (
      const CosNotification::EventTypeSeq & added,
      const CosNotification::EventTypeSeq & removed
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosNotifyComm::InvalidEventType
    ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
      const CosNotification::StructuredEvent & notification
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      CosEventComm::Disconnected
     ));

  virtual void disconnect_structured_push_consumer (
      ACE_ENV_SINGLE_ARG_DECL
      )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

  /// ProxyPushSupplier used to connect to NotifyLog.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The ID of the proxySupplier.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  /// The NotifyLog that this consumer connects to
  /// and whose events are to be logged.
  TAO_NotifyLog_i *log_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_NOTIFYLOGCONSUMER_H */
