//=============================================================================
/**
 *  @file   Notify_ConsumerAdmin_i.h
 *
 *  $Id$
 *
 * Implements the CosNotifyChannelAdmin::ConsumerAdmin interface.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_CONSUMERADMIN_I_H
#define TAO_NOTIFY_CONSUMERADMIN_I_H
#include "ace/pre.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "Notify_Event.h"
#include "Notify_Listeners.h"
#include "Notify_Collection.h"
#include "notify_export.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_Event_Manager;
class TAO_Notify_CO_Factory;
class TAO_Notify_POA_Factory;
class TAO_Notify_EMO_Factory;
class TAO_Notify_Collection_Factory;
class TAO_Notify_Worker_Task;
class TAO_Notify_Event_Processor;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_ConsumerAdmin_i
 *
 * @brief TAO_Notify_ConsumerAdmin_i
 *
 * Implements the CosNotifyChannelAdmin::ConsumerAdmin interface.
 */
class TAO_Notify_Export TAO_Notify_ConsumerAdmin_i : public TAO_Notify_EventListener, public POA_CosNotifyChannelAdmin::ConsumerAdmin, public PortableServer::RefCountServantBase
{

 public:
  /// Constructor
  /// <myChannel> is this objects parent.
  TAO_Notify_ConsumerAdmin_i (TAO_Notify_EventChannel_i* myChannel);

  /// Destructor
  virtual ~TAO_Notify_ConsumerAdmin_i (void);

  // = TAO_Notify_RefCounted
  virtual CORBA::ULong _incr_refcnt (void);
  virtual CORBA::ULong _decr_refcnt (void);

  // = The Servant methods
  virtual void _add_ref (ACE_ENV_SINGLE_ARG_DECL);
  virtual void _remove_ref (ACE_ENV_SINGLE_ARG_DECL);

  //= TAO_Notify_EventListener methods
  /// Callback methods to supply the event to the listener.
  virtual void dispatch_event (TAO_Notify_Event &event ACE_ENV_ARG_DECL);

  /// Evaluates true if this event is acceptable by the listener.
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean eval_parent ACE_ENV_ARG_DECL);

  /// Ask the listener to relinqish any bindings and prepare to be disposed.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// The Worker task associated with the event listener for event dispatching
  virtual TAO_Notify_Worker_Task* event_dispatch_task (void);

  /// The Worker task associated with the event listener for filter evaluation.
  ///= Admin Methods.
  virtual TAO_Notify_Worker_Task* filter_eval_task (void);

  /// Initialize the Consumer Admin.
  void init (CosNotifyChannelAdmin::AdminID myID,
             CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
             PortableServer::POA_ptr my_POA
             ACE_ENV_ARG_DECL);

  /// Return the CORBA object for this servant.
  CosNotifyChannelAdmin::ConsumerAdmin_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// Accesor for the event manager.
  TAO_Notify_Event_Manager* get_event_manager (void);

  /// Get our filter admin.
  TAO_Notify_FilterAdmin_i& get_filter_admin (void);

  /// Deactivate servant from <proxy_pushsupplier_POA_>.
  void deactivate_proxy_pushsupplier (PortableServer::Servant servant ACE_ENV_ARG_DECL);

  /// Register with parent for subscription updates.
  void register_listener (TAO_Notify_EventListener *listener ACE_ENV_ARG_DECL);

  /// Unregister with parent from subscription updates.
  void unregister_listener (TAO_Notify_EventListener *listener ACE_ENV_ARG_DECL);

  /// This id is no longer in use.It can be reused by <proxy_pushsupplier_ids_>
  void proxy_pushsupplier_destroyed (CosNotifyChannelAdmin::ProxyID proxyID);

  // = Interface methods
  virtual CosNotifyChannelAdmin::AdminID MyID (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr priority_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void priority_filter (
    CosNotifyFilter::MappingFilter_ptr priority_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyFilter::MappingFilter_ptr lifetime_filter (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void lifetime_filter (
    CosNotifyFilter::MappingFilter_ptr lifetime_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_suppliers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * push_suppliers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr get_proxy_supplier (
    CosNotifyChannelAdmin::ProxyID proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_pull_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual CosNotifyChannelAdmin::ProxySupplier_ptr obtain_notification_push_supplier (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual void destroy (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPushSupplier_ptr obtain_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullSupplier_ptr obtain_pull_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  /// Destroy CA
  void destroy_i (ACE_ENV_SINGLE_ARG_DECL);

 /// Obtain a proxy pushsupplier object
 CORBA::Object_ptr obtain_struct_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

 /// Obtain a structured proxy pushsupplier object.
 CORBA::Object_ptr obtain_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

 /// Obtain a sequence proxy pushsupplier object.
 CORBA::Object_ptr obtain_sequence_proxy_pushsupplier_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

 // = Data members
 /// The locking strategy.
 ACE_Lock* lock_;

 /// The reference count.
 CORBA::ULong refcount_;

 /// Flag to tell if the child poa's should be destroyed.
 CORBA::Boolean destory_child_POAs_;

 /// The channel to which we belong.
 TAO_Notify_EventChannel_i* event_channel_;

 /// The factory for channel objects.
 TAO_Notify_CO_Factory* channel_objects_factory_;

 /// The factory for POA based containers.
 TAO_Notify_POA_Factory* poa_factory_;

 /// Event manager objects factory,
 TAO_Notify_EMO_Factory* event_manager_objects_factory_;

 /// Collection objects factory
 TAO_Notify_Collection_Factory* collection_factory_;

 /// The event manager to use.
 TAO_Notify_Event_Manager* event_manager_;

 /// The inter filter operator to use.
 CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator_;

 /// My ID.
 CosNotifyChannelAdmin::AdminID my_id_;

 /// The POA in which we live.
 PortableServer::POA_var my_POA_;

 /// The POA in which all our push suppliers live.
 /// We create and own this POA.
 PortableServer::POA_var proxy_pushsupplier_POA_;

 /// The list of event types that all our proxys are interested in receiving.
 TAO_Notify_EventType_List subscription_list_;

 /// The list of event listeners that have registered with us
  TAO_Notify_EventListener_List* event_listener_list_;

  /// Id generator for proxy push suppliers.
  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID,
   CosNotifyChannelAdmin::ProxyIDSeq> proxy_pushsupplier_ids_;

  /// Handle QoS admin methods.
  TAO_Notify_QoSAdmin_i qos_admin_;

  /// Handles the Filter Admin methods.
  TAO_Notify_FilterAdmin_i filter_admin_;

  /// The dispatching task to send events to a listener group affiliated with this admin.
  TAO_Notify_Worker_Task* dispatching_task_;

  /// The filter evaluation task for this admin.
  TAO_Notify_Worker_Task* filter_eval_task_;
};

/****************************************************************************************************/

/**
 * @class TAO_Notify_Filter_Command_Worker
 *
 * @brief TAO_Notify_Filter_Command_Worker
 *
 * Enqueue each listener for the filter evaluation command.
 */
class TAO_Notify_Export TAO_Notify_Filter_Command_Worker : public TAO_ESF_Worker<TAO_Notify_EventListener>
{
public:
  TAO_Notify_Filter_Command_Worker (TAO_Notify_Event* event, TAO_Notify_Event_Processor* event_processor, CORBA::Boolean eval_parent);

  // = TAO_ESF_Worker method
  void work (TAO_Notify_EventListener* listener ACE_ENV_ARG_DECL);

protected:
  TAO_Notify_Event* event_;
  TAO_Notify_Event_Processor* event_processor_;
  CORBA::Boolean eval_parent_;
};

/****************************************************************************************************/

/**
 * @class TAO_Notify_Dispatch_Command_Worker
 *
 * @brief TAO_Notify_Dispatch_Command_Worker
 *
 * Worker to invoke the dispatch command for each member of the collection.
 */
class TAO_Notify_Export TAO_Notify_Dispatch_Command_Worker : public TAO_ESF_Worker<TAO_Notify_EventListener>
{
public:
  TAO_Notify_Dispatch_Command_Worker (TAO_Notify_Event* event, TAO_Notify_Event_Processor* event_processor);
  ~TAO_Notify_Dispatch_Command_Worker ();

  // = TAO_ESF_Worker method
  void work (TAO_Notify_EventListener* listener ACE_ENV_ARG_DECL);

protected:
  TAO_Notify_Event* event_;
  TAO_Notify_Event_Processor* event_processor_;
};

/****************************************************************************************************/

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_CONSUMERADMIN_I_H */
