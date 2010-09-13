// $Id$

// ============================================================================
//
// = FILENAME
//    DOVE_Supplier.h
//
// = DESCRIPTION
//    A wrapper around the event service initialization and marshalling.
//
// = AUTHORS
//    Michael Kircher (mk1@cs.wustl.edu)
//    Chris Gill (cdgill@cs.wustl.edu)
// ============================================================================

#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"


#ifndef DOVE_SUPPLIER_H
#define DOVE_SUPPLIER_H

#define SOURCE_ID 123

class DOVE_Supplier
{
public:

  DOVE_Supplier ();
  // Constructor.

  ~DOVE_Supplier (void);
  // Destructor.

  int init (void);
  // Initialize the ORB and the connection to the Name Service

  int connect (const char * MIB_name = 0,
               const char* es_name = 0,
               const char * ss_name = 0,
               ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info = 0);
  // Connect to the event service.

  void connected ();
  // Perform all post-connection-establishment processing.

  void disconnect ();
  // Disconnect from the event service.

  void notify (CORBA::Any& message);
  // Notify the consumer proxy that we have events

  void use_next_connection ();
  // Use the next connection in the list of established connections.

  void use_prev_connection ();
  // Use the previous connection in the list of established connections.

private:

  // The following Supplier is needed because DOVE_Supplier has not
  // inherited from PushSupplier.

  class Internal_DOVE_Supplier : public POA_RtecEventComm::PushSupplier
  {
    friend class DOVE_Supplier;

  public:

    virtual void disconnect_push_supplier (void)
    {
    }

  private:

    Internal_DOVE_Supplier (DOVE_Supplier* impl_ptr);

    DOVE_Supplier* impl_ptr_;
  };

private:

  struct Connection_Params
  {
    const char * es_name_;
    const char * ss_name_;
    RtecEventChannelAdmin::EventChannel_var eventChannel_var_;
    RtecEventChannelAdmin::SupplierAdmin_var supplierAdmin_var_;
    RtecEventChannelAdmin::ProxyPushConsumer_var proxyPushConsumer_var_;
    RtecScheduler::Scheduler_var scheduler_var_;
    RtecScheduler::handle_t rt_info_;
    ACE_Scheduler_Factory::POD_RT_Info pod_rt_info_;
  };

  int get_EventChannel ();
  // Get the event channel reference.

  int get_Scheduler ();
  // Get the scheduler reference.

  int connect_Supplier ();
  // Connect the the supplier with the event channel.

  static ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info_instance ();
  // Access the default rt_info singleton.

  static ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info_instance_;

  // Object initialization flags
  int initialized_;
  int connected_;

  Connection_Params ** connection_params_list_;
  Connection_Params * current_connection_params_;
  int connection_count_;
  int current_connection_index_;

  // = ORB, ORB Services adminstration references.
  CosNaming::NamingContext_var namingContext_var_;
  PortableServer::POA_var root_POA_var_;
  PortableServer::POAManager_var poa_manager_;

  Internal_DOVE_Supplier* internal_DOVE_Supplier_ptr_;

  const char * MIB_name_;
  // Keep the name of the MIB around.
};

#endif /* DOVE_SUPPLIER_H */

