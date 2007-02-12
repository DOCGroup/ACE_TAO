// $Id$

// ============================================================================
//
// = FILENAME
//    Event_Con.h
//
// = AUTHOR
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#ifndef EVENT_CON_H
#define EVENT_CON_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecSchedulerC.h"

class Demo_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //    Demo Consumer
  //
  // = DESCRIPTION
  //    Simple example of a consumer that registers for supplier
  //    notifications.
public:
  Demo_Consumer (void);

  int open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                     const char *my_name);
  // Uses the name server to obtain a reference to the <supplier_name>
  // and registers with channel to receive notifications from the
  // supplier.  Also registers to receive shutdown messages from the
  // supplier.  Stores <my_name> for printing out messages.  Returns 0
  // on success, -1 on failure.

  virtual void disconnect_push_consumer (void);
  // The channel is disconnecting.

  // = (not protected to allow short-circuiting) protected:
  virtual void push (const RtecEventComm::EventSet &events);
  // If the <events>[0] is a notification, prints out the data from
  // the supplier.  If its a shutdown message, the consumer
  // disconnects from the channel.

protected:
  void shutdown (void);
  // Disconnect from the Event Service.

  // = Event channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var suppliers_;

private:
  RtecScheduler::handle_t rt_info_;
};

#endif /* EVENT_CON_H */
