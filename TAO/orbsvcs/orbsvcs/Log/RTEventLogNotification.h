/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   RTEventLogNotification.h
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

#ifndef TLS_RTEVENT_LOG_NOTIFICATION_H
#define TLS_RTEVENT_LOG_NOTIFICATION_H
#include "ace/pre.h"

#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RTEventLogAdminS.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"

#include "rteventlog_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTEventLog_Export RTEventLogNotification : public LogNotification, 
                                                     POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Used to forward log generated events to a logging server
  //
  // = DESCRIPTION
  //   This implementation attempts to conform to the telecom
  //   logging specification.

public:
  // = Initialization and Termination
  RTEventLogNotification (RtecEventChannelAdmin::EventChannel_ptr);
  // Constructor.

  ~RTEventLogNotification (void);
  // Destructor.

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.


protected:
  // = Helper methods

  void obtainProxyConsumer (ACE_ENV_SINGLE_ARG_DECL);

  virtual void send_notification (const CORBA::Any& any)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Send the event on the event channel.


private:
  CORBA::ORB_var orb_;

  RtecEventChannelAdmin::EventChannel_var event_channel_;

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;
};
#include "ace/post.h"
#endif /* TLS_RTEVENT_LOG_NOTIFICATION_H */
