// $Id$
// =========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxySupplier.h
//
// = DESCRIPTION
//   Template Base class for all Proxy Suppliers.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_PROXY_SUPPLIER_T_H
#define TAO_NOTIFY_PROXY_SUPPLIER_T_H

#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "Notify_Listeners.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotificationS.h"
#include "ace/Containers_T.h"

class TAO_Notify_ConsumerAdmin_i;
class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

template <class SERVANT_TYPE>
class TAO_ORBSVCS_Export TAO_Notify_ProxySupplier : public SERVANT_TYPE, public TAO_Notify_Event_Listener, public TAO_Notify_Update_Listener
{
  // = TITLE
  //   TAO_Notify_ProxySupplier
  //
  // = DESCRIPTION
  //   The is a base class for all proxy suppliers, templatized by the servant
  //   type. All the Filter Admin and QoS Admin interface methods are
  //   implemented here by delegating to the admin implementations.
  //

public:
  TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumeradmin,
                            TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_ProxySupplier (void);
  // Destructor

  void init (CORBA::Environment &ACE_TRY_ENV);
  // Init

  // = Interface methods
  virtual void suspend_connection (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyInactive,
    CosNotifyChannelAdmin::NotConnected
  ));

virtual void resume_connection (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ConnectionAlreadyActive,
    CosNotifyChannelAdmin::NotConnected
  ));

virtual CosNotifyChannelAdmin::ProxyType MyType (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ConsumerAdmin_ptr MyAdmin (
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

virtual CosNotification::EventTypeSeq * obtain_offered_types (
    CosNotifyChannelAdmin::ObtainInfoMode mode,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void validate_event_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
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

virtual void subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

 protected:
// = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 void update_subscription_list_i (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, EVENTTYPE_LIST& added_ret, EVENTTYPE_LIST& removed_ret, CORBA::Environment &ACE_TRY_ENV);

 void subscribe_for_events_i (CORBA::Environment &ACE_TRY_ENV);
 // Derived classes should call this when they consumers connect.

 // = Data members
 CosNotifyChannelAdmin::ProxyType mytype_;
 // What type are we?

 TAO_Notify_ConsumerAdmin_i* myadmin_;
 // My parent consumer admin.

  TAO_Notify_Event_Manager* event_manager_;
  // Our event manager.

  TAO_Notify_Resource_Manager* resource_manager_;
  // The resource factory.

  EVENTTYPE_LIST subscription_list_;
  // A list of event types that we are interested in.

  CORBA::Boolean is_connected_;
  // True if we are connected to a consumer.

  CORBA::Boolean is_suspended_;
  // True if we are connected to a consumer and suspended.

  CORBA::Boolean is_destroyed_;
  // True if we are destroyed.

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_FilterAdmin_i filter_admin_;
  // Handles the Filter admin methods.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Notify_ProxySupplier_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Notify_ProxySupplier_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_PROXY_SUPPLIER_T_H */
