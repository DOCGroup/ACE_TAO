// $Id$

// ============================================================================
// 
// = FILENAME
//    Event_Sup.h
//
// = DESCRIPTION
//   Event Supplier for the simulation
//
// = AUTHOR
//    originally
//    David Levine (levine@cs.wustl.edu) and
//    Tim Harrison (harrison@cs.wustl.edu)
//    modified
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#if !defined (EVENT_SUP_H)
#define EVENT_SUP_H

#include <ctype.h>
#include <stdio.h>
#include "ace/OS.h"
#include "ace/SString.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "NavWeapC.h"

struct Schedule_Viewer_Data
{
  char operation_name [BUFSIZ];
  double utilitzation;
  double overhead;
  u_long arrival_time;
  u_long deadline_time;
  u_long completion_time;
  u_long computation_time;
};

class Demo_Supplier 
{
  // = TITLE
  //    Demo Supplier.
  //
  // = DESCRIPTION
  //    Generates event nofications and a shutdown message.
public:
  class Internal_Demo_Consumer : public POA_RtecEventComm::PushConsumer 
  {
  public:
    virtual void push (const RtecEventComm::EventSet &events,
		       CORBA::Environment &);
    // The channel pushed some events to us.

    virtual void disconnect_push_consumer (CORBA::Environment &);
    // The channel is disconnecting.

  private:
    Internal_Demo_Consumer (Demo_Supplier* impl);
    friend class Demo_Supplier;

  private:
    Demo_Supplier* impl_;
  };
  
  // The following Supplier is needed because Demo_Supplier has not
  // inherited from PushSupplier, therefore we register with the
  // following and send with Demo_Supplier.

  class Internal_Demo_Supplier : public POA_RtecEventComm::PushSupplier 
  {
  public:
    virtual void disconnect_push_supplier (CORBA::Environment &);

  private:
    Internal_Demo_Supplier (Demo_Supplier* impl);
    friend class Demo_Supplier;

  private:
    Demo_Supplier* impl_;
  };

  Demo_Supplier (u_int supplier_id);
  // Construction.  Requires the total number of messages to be
  // sent.  If the timestamp flag is enabled, then events are
  // timestamped, e.g., for use in measuring jitter.

  ~Demo_Supplier (void);
  
  int open_supplier (RtecEventChannelAdmin::EventChannel_ptr event_channel,
		     const char *name);
  // Registers with the name server under the given <name>.  Also
  // connects to the Event Channel as a supplier of notifications and
  // shutdown messages. 

  virtual void disconnect_push_supplier (CORBA::Environment &);
  // The channel is disconnecting.

  void start_generating_events (void);

  void load_schedule_data (ACE_Unbounded_Queue<Schedule_Viewer_Data *> &schedule_data);

  void insert_event_data (CORBA::Any &data, 
                          ACE_Unbounded_Queue_Iterator<Schedule_Viewer_Data *> &schedule_iter);

  void push (const RtecEventComm::EventSet &events,
	     CORBA::Environment &);
protected:

  void shutdown (void);
  // Disconnect from the Event Service.

  // = Event Channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var channel_admin_;
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin_;
  RtecEventChannelAdmin::ProxyPushConsumer_var proxy_consumer_;

  // = Because we get timeout events in our internal_demo_consumer.
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin_;
  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_supplier_;

private:
  Navigation navigation_;
  Weapons weapons_;
  RtecScheduler::handle_t rt_info_;
  u_int supplier_id_;

  Internal_Demo_Consumer *internal_demo_consumer_;
  Internal_Demo_Supplier *internal_demo_supplier_;
};

#endif /* EVENT_SUP_H */
