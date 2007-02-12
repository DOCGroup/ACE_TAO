// -*- C++ -*-

//=============================================================================
/**
 *  @file   EventLogNotification.h
 *
 *  $Id$
 *
 *  Methods to enable log-generated events.
 *
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 */
//=============================================================================

#ifndef TAO_TLS_EVENT_LOG_NOTIFICATION_H
#define TAO_TLS_EVENT_LOG_NOTIFICATION_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/Log/eventlog_serv_export.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EventLogNotification
 *
 * @brief Used to forward log generated events to a logging server.
 */
class TAO_EventLog_Serv_Export TAO_EventLogNotification :
  public TAO_LogNotification,
  public POA_CosEventComm::PushSupplier
{
public:

  // = Initialization and Termination methods.

  /// Constructor.
  TAO_EventLogNotification (CosEventChannelAdmin::EventChannel_ptr);

  /// Destructor.
  ~TAO_EventLogNotification (void);

  /// The skeleton methods.
  virtual void disconnect_push_supplier (void);

protected:
  // = Helper methods

  /// Used to connect this PushSupplier to the EventChannel
  /// that will log these events.
  void obtain_proxy_consumer ();

  /// Send the event on the event channel.
  virtual void send_notification (const CORBA::Any& any);


private:
  /// The ORB reference.
  CORBA::ORB_var orb_;

  /// The event channel we connect to.
  CosEventChannelAdmin::EventChannel_var event_channel_;

  /// The ProxyPushConsumer used to connect to the event channel.
  CosEventChannelAdmin::ProxyPushConsumer_var consumer_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TLS_EVENT_LOG_NOTIFICATION_H */
