/* -*- C++ -*- */
// $Id$

// ============================================================================
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   NotifyLogNotification_i.h
//
// = DESCRIPTION
//   Implementation of Log generated events for NotifyLog.
//
// = AUTHOR
//   D A Hanvey <d.hanvey@qub.ac.uk>
//
// ============================================================================

#ifndef TLS_NOTIFY_LOG_NOTIFICATION_H
#define TLS_NOTIFY_LOG_NOTIFICATION_H
#include "ace/pre.h"

#include "orbsvcs/DsNotifyLogAdminS.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/Notify/Notify_EventChannel_i.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNamingC.h"

#include "notifylog_export.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_NotifyLog_Export NotifyLogNotification : public LogNotification, 
  public POA_CosNotifyComm::PushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Used to forward log generated events to a logging server
  //
  // = DESCRIPTION
  //   This implementation attempts to conform to the telecom
  //   logging specification.

public:
  // = Initialization and Termination
  NotifyLogNotification (CosNotifyChannelAdmin::EventChannel_ptr);
  // Constructor.

  ~NotifyLogNotification (void);
  // Destructor.

protected:
  // = Helper methods
  // = Data members
  CosNotifyChannelAdmin::ProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This supplier's id.

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier method
    virtual void disconnect_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void send_notification (const CORBA::Any& any)
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  // Send the event on the event channel.


private:
  CORBA::ORB_var orb_;

  CosNotifyChannelAdmin::EventChannel_var event_channel_;
};
#include "ace/post.h"
#endif /* TLS_NOTIFY_LOG_NOTIFICATION_H */
