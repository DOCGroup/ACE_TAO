/* -*- C++ -*- */
/**
 *  @file Periodic_Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_PERIODIC_CONSUMER_H
#define TAO_NS_PERIODIC_CONSUMER_H
#include "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_StructuredPushConsumer.h"
#include "Task_Stats.h"

class TAO_NS_Task_Callback;
class ACE_Barrier;
class ACE_Arg_Shifter;

/**
 * @class TAO_NS_Periodic_Consumer
 *
 * @brief Receives Structured events periodically.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_NS_Periodic_Consumer : public TAO_Notify_StructuredPushConsumer
{
public:
  /// Constuctor
  TAO_NS_Periodic_Consumer (void);

  /// Destructor
  ~TAO_NS_Periodic_Consumer ();

  /// set the helper callback
  void task_callback(TAO_NS_Task_Callback* client);

  /// Init the state of this object.
  int init_state (ACE_Arg_Shifter& arg_shifter);

  /// Connect.
  void connect (CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

  /// connect
  void connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL);

  /// Subscription change.
  void subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL);

  /// dump stats
  void dump_stats (ACE_TCHAR* msg);

    /// Get the name of the proxy
  const char* proxy_name (void);

protected:

  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  /// = Data members.

  /// The name of the proxy we connect to.
  ACE_CString proxy_name_;

  /// count of events received.
  int count_;

  /// max events expected.
  /// Sometimes max count may not be the number of events that the supplier will send.
  /// A filter might not allow some events to through. we can tell the consumer what to expect.
  int max_count_;

  /// Stats house keeping
  Task_Stats stats_;

  /// Client
  TAO_NS_Task_Callback* client_;

  /// Flag to indicate if we want to check the expected priority.
  int check_priority_;
};

#include "ace/post.h"
#endif /* TAO_NS_PERIODIC_CONSUMER_H */
