// $Id$
// ============================================================================
//
// = FILENAME
//    DOVEMIB.h
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#ifndef DOVE_MIB_H
#define DOVE_MIB_H

//#include "ace/SString.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "AnyAnalyser.h"

class MIB_Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //    Demo Consumer
  //
  // = DESCRIPTION
  //    Simple example of a consumer that registers for supplier
  //    notifications.
public:
  MIB_Consumer (void);

  int open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                     const char *my_name);
  // Uses the name server to obtain a reference to the <supplier_name>
  // and registers with channel to receive notifications from the
  // supplier.  Also registers to receive shutdown messages from the
  // supplier.  Stores <my_name> for printing out messages.  Returns 0
  // on success, -1 on failure.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  // = (not protected to allow short-circuiting) protected:
  virtual void push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &);
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
  AnyAnalyser anyAnalyser_;
private:
  RtecScheduler::handle_t rt_info_;
};

#endif /* DOVE_MIB_H */
