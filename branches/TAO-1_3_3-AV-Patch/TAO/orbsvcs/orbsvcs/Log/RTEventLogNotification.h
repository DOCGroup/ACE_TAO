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

#ifndef TAO_TLS_RTEVENT_LOG_NOTIFICATION_H
#define TAO_TLS_RTEVENT_LOG_NOTIFICATION_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_ConsumerAdmin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RTEventLogAdminS.h"
#include "orbsvcs/Event/EC_Default_Factory.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h"

#include "rteventlog_export.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_RTEventLogNotification
 *
 * @brief Used to forward log generated events to a logging server.
 */
class TAO_RTEventLog_Export TAO_RTEventLogNotification : public TAO_LogNotification, 
                                                         POA_RtecEventComm::PushSupplier
{
public:
  // = Initialization and Termination methods.

  /// Constructor.
  TAO_RTEventLogNotification (RtecEventChannelAdmin::EventChannel_ptr);

  /// Destructor.
  ~TAO_RTEventLogNotification (void);

  /// The skeleton methods.
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

protected:
  // = Helper methods

  /// Used to connect this PushSupplier to the EventChannel
  /// that will log these events.
  void obtainProxyConsumer (ACE_ENV_SINGLE_ARG_DECL);

  // Send the event on the event channel.
  virtual void send_notification (const CORBA::Any& any)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// The ORB reference.
  CORBA::ORB_var orb_;

  /// The event channel we connect to.
  RtecEventChannelAdmin::EventChannel_var event_channel_;

  /// The ProxyPushConsumer used to connect to the event channel.
  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_TLS_RTEVENT_LOG_NOTIFICATION_H */
