/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   EventLogNotification.h
 *
 *  $Id$
 *
 *  Methods to enable log-generated events.
 *
 *
 *  @author D A Hanvey <d.hanvey@qub.ac.uk>
 *   
 *  
 */
//=============================================================================

#ifndef TLS_EVENT_LOG_NOTIFICATION_H
#define TLS_EVENT_LOG_NOTIFICATION_H
#include "ace/pre.h"

#include "orbsvcs/CosEvent/CEC_ConsumerAdmin.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/DsEventLogAdminS.h"
#include "orbsvcs/CosEvent/CEC_Default_Factory.h"
#include "orbsvcs/CosEventCommS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EventLog_Export EventLogNotification : public LogNotification, 
                                                 POA_CosEventComm::PushSupplier
{
  // = TITLE
  //   Used to forward log generated events to a logging server
  //
  // = DESCRIPTION
  //   This implementation attempts to conform to the telecom
  //   logging specification.

public:
  // = Initialization and Termination
  EventLogNotification (CosEventChannelAdmin::EventChannel_ptr);
  // Constructor.

  ~EventLogNotification (void);
  // Destructor.

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

protected:
  // = Helper methods

  void obtain_proxy_consumer ();

  virtual void send_notification (const CORBA::Any& any ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send the event on the event channel.

private:
  CORBA::ORB_var orb_;
  // The ORB.

  CosEventChannelAdmin::EventChannel_var event_channel_;
  // The event channel we connect to.

  CosEventChannelAdmin::ProxyPushConsumer_var consumer_;
  // The ProxyPushConsumer used to connect to the event channel.
};
#include "ace/post.h"
#endif /* TLS_EVENT_LOG_NOTIFICATION_H */
