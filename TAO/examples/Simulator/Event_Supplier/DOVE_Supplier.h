// $Id$

// ============================================================================
//
// = FILENAME
//    DOVE_Supplier.cpp
//
// = DESCRIPTION
//    A wrapper around the event service initialization and marshalling.
//
// = AUTHORS
//    Michael Kircher (mk1@cs.wustl.edu)
//    Chris Gill (cdgill@cs.wustl.edu)
// ============================================================================

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
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
               ACE_Scheduler_Factory::POD_RT_Info * rt_info = 0);
  // Connect to the event service.

  void disconnect ();
  // Disconnect from the event service.

  void notify (CORBA::Any& message);

  // The following Supplier is needed because DOVE_Supplier has not
  // inherited from PushSupplier.

private:

  class Internal_DOVE_Supplier : public POA_RtecEventComm::PushSupplier
  {
    friend class DOVE_Supplier;

  public:

    virtual void disconnect_push_supplier (CORBA::Environment &)
    {
    }

  private:

    Internal_DOVE_Supplier (DOVE_Supplier* impl_ptr);

    DOVE_Supplier* impl_ptr_;
  };

private:

  int get_EventChannel ();
  // Get the event channel reference.

  int get_Scheduler ();
  // Get the scheduler reference.

  int connect_Supplier (ACE_Scheduler_Factory::POD_RT_Info * rt_info = 0);
  // Connect the the supplier with the event channel.

  static ACE_Scheduler_Factory::POD_RT_Info * rt_info_instance ();
  // Access the default rt_info singleton.

  static ACE_Scheduler_Factory::POD_RT_Info * rt_info_instance_;

  int initialized_;

  // = ORB, ORB Services adminstration references.

  RtecEventChannelAdmin::EventChannel_var eventChannel_var_;
  RtecEventChannelAdmin::SupplierAdmin_var supplierAdmin_var_;
  RtecEventChannelAdmin::ProxyPushConsumer_var proxyPushConsumer_var_;
  RtecScheduler::Scheduler_var scheduler_var_;
  RtecScheduler::handle_t rt_info_;
  CosNaming::NamingContext_var namingContext_var_;
  PortableServer::POA_var root_POA_var_;
  PortableServer::POAManager_var poa_manager_;


  Internal_DOVE_Supplier* internal_DOVE_Supplier_ptr_;

  const char * MIB_name_;
  // Keep the name of the MIB around.

  const char * es_name_;
  // Keep the name of the event service around.

  const char * ss_name_;
  // Keep the name of the scheduling service around.
};

#endif /* DOVE_SUPPLIER_H */

