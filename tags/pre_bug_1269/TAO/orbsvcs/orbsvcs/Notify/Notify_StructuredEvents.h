/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_StructuredEvents.h
 *
 *  $Id$
 *
 * For use with the Sequence (Push/Pull)(Consumer/Supplier)
 *
 *
 *  @author Chad Elliott <elliott_c@ociweb.com>
 */
//=============================================================================


#ifndef TAO_NOTIFY_STRUCTURED_EVENTS_H
#define TAO_NOTIFY_STRUCTURED_EVENT_H

#include "ace/pre.h"
#include "orbsvcs/Notify/Notify_Event.h"
#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Notify_StructuredEvents
 *
 * @brief TAO_Notify_StructuredEvents
 *
 * This class is the concrete prototype for the Structured Event Types.
 */
class TAO_Notify_Export TAO_Notify_StructuredEvents : public TAO_Notify_Event
{
public:
  TAO_Notify_StructuredEvents (const CosNotification::EventBatch& notifications);
  virtual ~TAO_Notify_StructuredEvents ();

  virtual TAO_Notify_Event* clone (void);
  void operator=(const TAO_Notify_StructuredEvents& structured_events);

  virtual CORBA::Boolean is_special_event_type (void) const;
  virtual const TAO_Notify_EventType& event_type (void) const;
  virtual CORBA::Boolean do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL);
  virtual void do_push (CosEventComm::PushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer ACE_ENV_ARG_DECL) const;
  virtual void do_push (CosNotifyComm::SequencePushConsumer_ptr consumer,
                        const TAO_Notify_QoSAdmin_i& qos_admin,
                        CosNotification::EventBatch& unsent,
                        int flush_queue
                        ACE_ENV_ARG_DECL) const;

protected:

  /// Load the QoS properties these events from <data_>.
  void init_QoS (void);

  // = Data Members
  /// The data
  CosNotification::EventBatch data_;

  /// The events that matched the filter
  TAO_Unbounded_Sequence<CORBA::ULong> matching_;

  /// Was do_match () called?
  int match_called_;

  /// The event types of <data_>
  TAO_Notify_EventType event_type_;
};

#include "ace/post.h"

#endif /* TAO_NOTIFY_EVENT_H */
