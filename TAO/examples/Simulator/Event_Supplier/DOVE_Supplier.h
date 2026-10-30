
//=============================================================================
/**
 *  @file    DOVE_Supplier.h
 *
 *  A wrapper around the event service initialization and marshalling.
 *
 *  @author Michael Kircher (mk1@cs.wustl.edu) Chris Gill (cdgill@cs.wustl.edu)
 */
//=============================================================================


#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"

#include <cstddef>
#include <memory>
#include <vector>


#ifndef DOVE_SUPPLIER_H
#define DOVE_SUPPLIER_H

#define SOURCE_ID 123

class DOVE_Supplier
{
public:
  /// Constructor.
  DOVE_Supplier ();

  /// Destructor.
  ~DOVE_Supplier () = default;

  DOVE_Supplier (DOVE_Supplier const &) = delete;
  DOVE_Supplier (DOVE_Supplier &&) = delete;
  DOVE_Supplier &operator= (DOVE_Supplier const &) = delete;
  DOVE_Supplier &operator= (DOVE_Supplier &&) = delete;

  /// Initialize the ORB and the connection to the Name Service
  int init ();

  /// Connect to the event service.
  int connect (char const *MIB_name = nullptr,
               char const *es_name = nullptr,
               char const *ss_name = nullptr,
               ACE_Scheduler_Factory::POD_RT_Info *pod_rt_info = nullptr);

  /// Perform all post-connection-establishment processing.
  void connected ();

  /// Disconnect from the event service.
  void disconnect ();

  /// Notify the consumer proxy that we have events
  void notify (CORBA::Any& message);

  /// Use the next connection in the list of established connections.
  void use_next_connection ();

  /// Use the previous connection in the list of established connections.
  void use_prev_connection ();

private:
  // The following Supplier is needed because DOVE_Supplier has not
  // inherited from PushSupplier.

  class Internal_DOVE_Supplier : public POA_RtecEventComm::PushSupplier
  {
    friend class DOVE_Supplier;

  public:
    void disconnect_push_supplier () override
    {
    }

  private:
    explicit Internal_DOVE_Supplier (DOVE_Supplier *impl_ptr);

    DOVE_Supplier *impl_ptr_;
  };

private:
  struct Connection_Params
  {
    char const *es_name_;
    char const *ss_name_;
    RtecEventChannelAdmin::EventChannel_var eventChannel_var_;
    RtecEventChannelAdmin::SupplierAdmin_var supplierAdmin_var_;
    RtecEventChannelAdmin::ProxyPushConsumer_var proxyPushConsumer_var_;
    RtecScheduler::Scheduler_var scheduler_var_;
    RtecScheduler::handle_t rt_info_;
    ACE_Scheduler_Factory::POD_RT_Info pod_rt_info_;
  };

  /// Get the event channel reference.
  int get_EventChannel ();

  /// Get the scheduler reference.
  int get_Scheduler ();

  /// Connect the the supplier with the event channel.
  int connect_Supplier ();

  /// Access the default rt_info singleton.
  static ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info_instance ();

  static ACE_Scheduler_Factory::POD_RT_Info * pod_rt_info_instance_;

  // Object initialization flags
  bool initialized_ {};
  bool connected_ {};

  std::vector<std::unique_ptr<Connection_Params>> connection_params_list_;
  Connection_Params *current_connection_params_ {};
  std::size_t current_connection_index_ {};

  // = ORB, ORB Services adminstration references.
  CosNaming::NamingContext_var namingContext_var_;
  PortableServer::POA_var root_POA_var_;
  PortableServer::POAManager_var poa_manager_;

  std::unique_ptr<Internal_DOVE_Supplier> internal_DOVE_Supplier_ptr_;

  /// Keep the name of the MIB around.
  char const *MIB_name_ {};
};

#endif /* DOVE_SUPPLIER_H */

