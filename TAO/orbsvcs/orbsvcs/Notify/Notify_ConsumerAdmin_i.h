// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ConsumerAdmin_i.h
//
// = DESCRIPTION
//   Implements the CosNotifyChannelAdmin::ConsumerAdmin interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_CONSUMERADMIN_I_H
#include "ace/pre.h"
#define TAO_NOTIFY_CONSUMERADMIN_I_H

#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "Notify_Types.h"
#include "Notify_Listeners.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "notify_export.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_ConsumerAdmin_i : public POA_CosNotifyChannelAdmin::ConsumerAdmin, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_ConsumerAdmin_i
  //
  // = DESCRIPTION
  //   Implements the CosNotifyChannelAdmin::ConsumerAdmin interface.
  //

 public:
  TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i* myChannel, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor
  // <myChannel> is this objects parent.

  virtual ~TAO_Notify_ConsumerAdmin_i (void);
  // Destructor

  void init (CosNotifyChannelAdmin::AdminID myID,
             CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
             PortableServer::POA_ptr my_POA,
             CORBA::Environment &ACE_TRY_ENV);
  // Initialize the Consumer Admin.

  CosNotifyChannelAdmin::ConsumerAdmin_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Return the CORBA object for this servant.

  TAO_Notify_Event_Manager* get_event_manager (void);
  // Accesor for the event manager.

  TAO_Notify_FilterAdmin_i& get_filter_admin (void);
  // Get our filter admin.

  void deactivate_proxy_pushsupplier (PortableServer::Servant servant, CORBA::Environment &ACE_TRY_ENV);
  // Deactivate servant from <proxy_pushsupplier_POA_>.

  void register_listener (TAO_Notify_Event_Listener *listener, CORBA::Environment &ACE_TRY_ENV);
  // Register with parent for subscription updates.

  void unregister_listener (TAO_Notify_Event_Listener *listener, CORBA::Environment &ACE_TRY_ENV);
  // Unregister with parent for subscription updates.

  // = Interface methods
  virtual CosNotifyChannelAdmin::AdminID MyID (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_pull_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_push_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual void destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
// = Helper methods
 void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 CORBA::Object_ptr obtain_struct_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
 // Obtain a proxy pushsupplier object

 CORBA::Object_ptr obtain_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
 // Obtain a structured proxy pushsupplier object.

 CORBA::Object_ptr obtain_sequence_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
 // Obtain a sequence proxy pushsupplier object.

 // = Data members
  TAO_Notify_EventChannel_i* my_channel_;
  // The channel to which we belong.

  TAO_Notify_Resource_Manager* resource_manager_;
  // The resource factory that we use.

  TAO_Notify_Event_Manager* event_manager_;
  // The event manager to use.

  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator_;
  // The inter filter operator to use.

  CosNotifyChannelAdmin::AdminID myID_;
  // My ID.

  PortableServer::POA_var my_POA_;
  // The POA in which we live.

  PortableServer::POA_var proxy_pushsupplier_POA_;
  // The POA in which all our push suppliers live.
  // We create and own this POA.

  EVENTTYPE_LIST subscription_list_;
  // The list of event types that all our interested in receiving.

  EVENT_LISTENER_LIST event_listener_list_;
  // The list of event listeners that have registered with us

  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID,
    CosNotifyChannelAdmin::ProxyIDSeq> proxy_pushsupplier_ids_;
  // Id generator for proxy push suppliers.

  CORBA::Boolean is_destroyed_;
  // Are we dead?

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_FilterAdmin_i filter_admin_;
  // Handles the Filter Admin methods.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_CONSUMERADMIN_I_H */
