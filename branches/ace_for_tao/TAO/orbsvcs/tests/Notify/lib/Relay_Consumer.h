/* -*- C++ -*- */
/**
 *  @file Relay_Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_RELAY_CONSUMER_H
#define TAO_Notify_Tests_RELAY_CONSUMER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Direct_Consumer.h"

/**
 * @class TAO_Notify_Tests_Relay_Consumer
 *
 * @brief Relay events to a destination consumer.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Relay_Consumer : public TAO_Notify_Tests_Direct_Consumer
{
public:
  /// Constuctor
  TAO_Notify_Tests_Relay_Consumer (ACE_CString& destination);

  /// Destructor
  ~TAO_Notify_Tests_Relay_Consumer ();

  /// Connect using options parsed and set initial QoS.
  virtual void connect (ACE_ENV_SINGLE_ARG_DECL);

  void push_structured_event (const CosNotification::StructuredEvent &/*notification*/
                              ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventComm::Disconnected));

protected:
  /// Destination object.
  ACE_CString destination_;

  /// This object is resolved in the connect method.
  CosNotifyComm::StructuredPushConsumer_var destination_object_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_RELAY_CONSUMER_H */
