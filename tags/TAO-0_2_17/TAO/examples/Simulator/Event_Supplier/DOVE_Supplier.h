// $Id$

// ============================================================================
//
// = FILENAME
//    DOVE_Supplier.cpp
//
// = DESCRIPTION
//    A wrapper around the event servie initialization and marshalling.
//
// = AUTHOR
//    Michael Kircher (mk1@cs.wustl.edu)
//
// ============================================================================

#include "ace/OS.h"
#include "tao/corba.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"


#if !defined (DOVE_SUPPLIER_H)
#define DOVE_SUPPLIER_H

#define SOURCE_ID 123

class DOVE_Supplier 
{
public:
  DOVE_Supplier ();

  ~DOVE_Supplier (void);

  int connect (const char * MIB_name);

  void disconnect ();

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
  // get a reference to the event channel
  
  int connect_Supplier ();
  // connect the the supplier with the event channel

  // = Event Channel adminstration references.
  RtecEventChannelAdmin::EventChannel_var eventChannel_var_;
  RtecEventChannelAdmin::SupplierAdmin_var supplierAdmin_var_;
  RtecEventChannelAdmin::ProxyPushConsumer_var proxyPushConsumer_var_;
  RtecScheduler::Scheduler_var scheduler_var_; 

  Internal_DOVE_Supplier* internal_DOVE_Supplier_ptr_;

  const char * MIB_name_;
  // keep the name of the MIB around
};

#endif /* DOVE_SUPPLIER_H */

